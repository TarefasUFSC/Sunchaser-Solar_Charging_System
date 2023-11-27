#ifndef TIMERINTERRUPT_H
#define TIMERINTERRUPT_H

#include <Arduino.h>
#include <DateTime.h>
#include "SaveToFlash.h"
#include "Sensors.h"


class TimerInterrupt{
  private:
    hw_timer_t * timer;
    static volatile SemaphoreHandle_t timerSemaphore;
    int QtdMinutes;
    SaveToFlash* fileSystem;
    Sensors* read_sensors;
  public:
    TimerInterrupt(SaveToFlash *files, Sensors *sensors); // Constructor
    static void IRAM_ATTR onTimer();
    void timer_init();
    void timer_interruption();
    void set_newtime(int newTime);
};

#endif
