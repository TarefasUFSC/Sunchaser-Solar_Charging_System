#ifndef TIMERINTERRUPT_H
#define TIMERINTERRUPT_H

#include <Arduino.h>
#include <DateTime.h>
#include "SaveToFlash.h"
#include "communicator.h"
#include "Sensors.h"
#include "TimeConfigurations.h"

class TimerInterrupt
{
private:
  hw_timer_t *timer;
  static volatile SemaphoreHandle_t timerSemaphore;
  Sensors *read_sensors;
  SaveToFlash *fileSystem;
  TimeConfigurations *timeConfigs;
  Communicator *communicator;

public:
  TimerInterrupt(SaveToFlash *files, Communicator *communicator, Sensors *sensors, TimeConfigurations *configs); // Constructor
  static void IRAM_ATTR onTimer();
  void timer_init();
  void timer_interruption();
  bool tryToSendCacheToServer();
};

#endif
