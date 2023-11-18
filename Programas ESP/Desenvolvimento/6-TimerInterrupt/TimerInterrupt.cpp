#include "TimerInterrupt.h"

//Initialize the static variables
volatile SemaphoreHandle_t TimerInterrupt::timerSemaphore = xSemaphoreCreateBinary();

TimerInterrupt::TimerInterrupt(){
  timer = NULL;
  QtdMinutes = 60;  // Default time is 60 minutes
  CacheSize = 24;  // Default cache size is 24
}

void IRAM_ATTR TimerInterrupt::onTimer(){ // It is safe to use digitalRead/Write here if you want to toggle an output
  xSemaphoreGiveFromISR(timerSemaphore, NULL); // Give a semaphore that we can check in the loop
}

void TimerInterrupt::timer_init(){
  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more info).
  timer = timerBegin(0, 80, true);

  timerAttachInterrupt(timer, &TimerInterrupt::onTimer, true); // Attach onTimer function to our timer.

  int SetTime = QtdMinutes*60*1000000; // Set alarm to call onTimer function every QtdMinutes
  timerAlarmWrite(timer, SetTime, true); // Repeat the alarm (third parameter)
  
  timerAlarmEnable(timer); // Start an alarm
}

void TimerInterrupt::timer_interruption(){ // If Timer has fired
  float time, BatteryCurrent, BatteryVoltage, PVCurrent, PVVoltage;
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    Serial.println("entering timer interrupt");
    // Read the sensors
    time = random(100);
    BatteryCurrent = random(100);
    BatteryVoltage = random(100);
    PVCurrent = random(100);
    PVVoltage = random(100);

    // Save the readings to flash
    String JSON = createJSON("BatteryCurrent", BatteryCurrent, time);
    appendFile(LittleFS, "/readings/data.dt", JSON);
    JSON = createJSON("BatteryVoltage", BatteryVoltage, time);
    appendFile(LittleFS, "/readings/data.json", JSON);
    JSON = createJSON("PVCurrent", PVCurrent, time);
    appendFile(LittleFS, "/readings/data.json", JSON);
    JSON = createJSON("PVVoltage", PVVoltage, time);
    appendFile(LittleFS, "/readings/data.json", JSON);

    // Read the file
    readFile(LittleFS, "/readings/data.json");
  }
}

void TimerInterrupt::set_newtime(int newTime){
  QtdMinutes = newTime;
}

void TimerInterrupt::set_newcachesize(int newSize){
  CacheSize = newSize;
}