#include "TimerInterrupt.h"
#include "SaveToFlash.h"

SaveToFlash fileSystem;
TimerInterrupt timerInterrupt(fileSystem);

void setup(){
    Serial.begin(115200);
    timerInterrupt.timer_init();
    fileSystem.mountLittleFS();
}

void loop(){
    timerInterrupt.timer_interruption();
}
