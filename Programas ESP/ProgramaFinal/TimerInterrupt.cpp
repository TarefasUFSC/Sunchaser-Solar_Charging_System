#include "TimerInterrupt.h"
#include <math.h>

// Initialize the static variables
volatile SemaphoreHandle_t TimerInterrupt::timerSemaphore = xSemaphoreCreateBinary();

TimerInterrupt::TimerInterrupt(SaveToFlash *files, Communicator *communicator, Sensors *sensors, TimeConfigurations *configs)
{
  timer = NULL;
  this->fileSystem = files;
  this->communicator = communicator;
  this->read_sensors = sensors;
  this->timeConfigs = configs;
}

void IRAM_ATTR TimerInterrupt::onTimer()
{                                              // It is safe to use digitalRead/Write here if you want to toggle an output
  xSemaphoreGiveFromISR(timerSemaphore, NULL); // Give a semaphore that we can check in the loop
}

void TimerInterrupt::timer_init()
{
  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more info).
  timer = timerBegin(0, 80, true);

  timerAttachInterrupt(timer, &TimerInterrupt::onTimer, true); // Attach onTimer function to our timer.

  int QtdMinutes = timeConfigs->get_ReadingInterval();
  int SetTime = QtdMinutes * 10 * 1000000; // Set alarm to call onTimer function every QtdMinutes
  timerAlarmWrite(timer, SetTime, true);   // Repeat the alarm (third parameter)

  timerAlarmEnable(timer); // Start an alarm
}

bool TimerInterrupt::tryToSendCacheToServer()
{
  bool sent = true;
  int qtd_cache = this->fileSystem->getNCacheSaves();
  int pages = ceil((float)qtd_cache / (float)NUM_READINGS);

  this->communicator->reconnect_client(); // chama isso pra acordar o cliente e reconectar com o broker -> só funciona se o esp estiver em modo client
  for (int i = 0; i < pages; i++)
  {
    Readings_Lists readings = this->fileSystem->get_readings_from_cache(i);
    if (!(this->communicator->send_data_to_server(readings)))
    {
      sent = false;
      break;
    }
  }
  this->communicator->sleep();
  return sent;
}

void TimerInterrupt::timer_interruption()
{ // If Timer has fired
  String time;
  float BatteryCurrent, BatteryVoltage, PVCurrent;
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE)
  {
    Serial.println("entering timer interrupt");

    // Read the sensors
    time = DateTime.toISOString().c_str();
    BatteryCurrent = read_sensors->battery_current();
    BatteryVoltage = read_sensors->battery_voltage();
    PVCurrent = read_sensors->pv_current();

    fileSystem->saveToCache(time, BatteryCurrent, BatteryVoltage, PVCurrent);
    int n_cache_saves = fileSystem->getNCacheSaves();
    int max_cache_size = fileSystem->getMaxCacheSize();
    // If the cache is full, save it to the long term memory
    Serial.printf("n_cache: %d | max_cache_size: %d\n", n_cache_saves, max_cache_size);
    if (n_cache_saves >= max_cache_size && this->tryToSendCacheToServer())
    {
      fileSystem->saveToLongTerm();
    }
    //    Readings_Lists readings = fileSystem->get_readings_/from_cache(0);
  }
}
