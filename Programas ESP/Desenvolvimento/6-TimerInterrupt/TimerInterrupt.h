#ifndef TIMERINTERRUPT_H
#define TIMERINTERRUPT_H

#include <Arduino.h>
#include "Sensors.h"


class TimerInterrupt{
  public:
    TimerInterrupt();
    static void IRAM_ATTR onTimer();
    void timer_init();
    void timer_interruption();
    void set_time(int newTime);
  private:
    hw_timer_t * timer;
    static volatile SemaphoreHandle_t timerSemaphore;
    int SetMinutes;
};

//create a struct to hold the data
struct SensorsData {
  float BatteryCurrent;
  float BatteryVoltage;
  float PVCurrent;
  float PVVoltage;
};

#endif
