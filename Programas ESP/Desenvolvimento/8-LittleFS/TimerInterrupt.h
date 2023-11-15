#ifndef TIMERINTERRUPT_H
#define TIMERINTERRUPT_H

#include <Arduino.h>
#include "SaveToFlash.h"


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

#endif
