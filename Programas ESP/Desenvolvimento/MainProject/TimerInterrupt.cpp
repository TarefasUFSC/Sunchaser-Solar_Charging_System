#include "TimerInterrupt.h"

// Initialize the static variables
volatile SemaphoreHandle_t TimerInterrupt::timerSemaphore = xSemaphoreCreateBinary();

TimerInterrupt::TimerInterrupt(SaveToFlash *files, Sensors *sensors, TimeConfigurations &configs)
{
  timer = NULL;
  timeConfigs = configs;
  fileSystem = files;
  read_sensors = sensors;
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

  int QtdMinutes = timeConfigs.get_ReadingInterval();
  int SetTime = QtdMinutes * 10 * 1000000; // Set alarm to call onTimer function every QtdMinutes
  timerAlarmWrite(timer, SetTime, true);   // Repeat the alarm (third parameter)

  timerAlarmEnable(timer); // Start an alarm
}

void TimerInterrupt::timer_interruption()
{ // If Timer has fired
  String date_time;
  float BatteryCurrent, BatteryVoltage, PVCurrent, PVVoltage;
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE)
  {
    Serial.println("entering timer interrupt");

    // Read the sensors
    date_time = DateTime.toISOString().c_str();
    BatteryCurrent = read_sensors->battery_current();
    BatteryVoltage = read_sensors->battery_voltage();
    PVCurrent = read_sensors->pv_current();

    fileSystem->saveToCache(date_time, BatteryCurrent, BatteryVoltage, PVCurrent);

    // If the cache is full, save it to the long term memory
    int n_cache_saves = fileSystem->getNCacheSaves();
    int cache_size = timeConfigs.get_CacheMaxSize();
    Serial.printf("n_cache: %d | cache_size: %d\n", n_cache_saves, cache_size);
    if (n_cache_saves >= cache_size)
    {
      fileSystem->saveToLongTerm();
    }
  }
}
