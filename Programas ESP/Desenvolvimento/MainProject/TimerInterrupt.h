#ifndef TIMERINTERRUPT_H
#define TIMERINTERRUPT_H

#include <Arduino.h>
#include <DateTime.h>
#include "SaveToFlash.h"
#include "Sensors.h"
#include "TimeConfigurations.h"


class TimerInterrupt{
  private:
    hw_timer_t * timer;
    static volatile SemaphoreHandle_t timerSemaphore;
    Sensors* read_sensors;
    SaveToFlash* fileSystem;
    TimeConfigurations timeConfigs;
  public:
    TimerInterrupt(SaveToFlash *files, Sensors *sensors, TimeConfigurations &configs); // Constructor
    static void IRAM_ATTR onTimer();
    void timer_init();
    void timer_interruption();
};

#endif
