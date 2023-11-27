#ifndef TIMERINTERRUPT_H
#define TIMERINTERRUPT_H

#include <Arduino.h>
#include "SaveToFlash.h"


class TimerInterrupt{
  private:
    hw_timer_t * timer;
    static volatile SemaphoreHandle_t timerSemaphore;
    int QtdMinutes;
    SaveToFlash* fileSystem;
  public:
    TimerInterrupt(SaveToFlash *files); // Constructor
    static void IRAM_ATTR onTimer();
    void timer_init();
    void timer_interruption();
    void set_newtime(int newTime);
};

#endif
