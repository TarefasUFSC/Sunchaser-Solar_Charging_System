#include "TimerInterrupt.h"
#include "SaveToFlash.h"

TimerInterrupt timerInterrupt = TimerInterrupt();

void setup(){
    Serial.begin(115200);
    timerInterrupt.timer_init();
    mountLittleFS();
    
    listDir(LittleFS, "/", 3);
}

//int count = 0;

void loop(){
    timerInterrupt.timer_interruption();
}
