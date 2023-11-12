#include "TimerInterrupt.h"
#include "Sensors.h"

Adafruit_INA219 ina219; // Current sensor declaration

TimerInterrupt timerInterrupt = TimerInterrupt();

void setup() {
  Serial.begin(115200);

  timerInterrupt.timer_init();

}

void loop() {
  timerInterrupt.timer_interruption();

}
