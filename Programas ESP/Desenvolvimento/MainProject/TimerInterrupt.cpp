#include "TimerInterrupt.h"

//Initialize the static variables
volatile SemaphoreHandle_t TimerInterrupt::timerSemaphore = xSemaphoreCreateBinary();

TimerInterrupt::TimerInterrupt(SaveToFlash &files){
  timer = NULL;
  QtdMinutes = 60;  // Default time is 60 minutes
  fileSystem = files;
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
    int n_cache_saves = fileSystem.getNCacheSaves();
    int cache_size = fileSystem.getCachesize();
    // If the cache is full, save it to the long term memory
    if (n_cache_saves >= cache_size){
      fileSystem.saveToLongTerm();
    }
  }
}

void TimerInterrupt::set_newtime(int newTime){
  QtdMinutes = newTime;
}