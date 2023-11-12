#include "TimerInterrupt.h"

//create a array of structs and an index to keep track of the readings
#define NUM_READINGS 12
SensorsData Readings[NUM_READINGS];
int ArrayIndex = 0;

extern Adafruit_INA219 ina219; // Sensor de corrente declarado no arquivo principal

//Initialize the static variables
volatile SemaphoreHandle_t TimerInterrupt::timerSemaphore = xSemaphoreCreateBinary();

TimerInterrupt::TimerInterrupt(){
  timer = NULL;
  SetMinutes = 1;
}

void IRAM_ATTR TimerInterrupt::onTimer(){ // It is safe to use digitalRead/Write here if you want to toggle an output
  xSemaphoreGiveFromISR(timerSemaphore, NULL); // Give a semaphore that we can check in the loop
}

void TimerInterrupt::timer_init(){
  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more info).
  timer = timerBegin(0, 80, true);

  timerAttachInterrupt(timer, &TimerInterrupt::onTimer, true); // Attach onTimer function to our timer.

  int SetTime = SetMinutes*60*1000000; // Set alarm to call onTimer function every SetMinutes
  timerAlarmWrite(timer, SetTime, true); // Repeat the alarm (third parameter)
  
  timerAlarmEnable(timer); // Start an alarm
}

void TimerInterrupt::timer_interruption(){ // If Timer has fired
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    Serial.print("entering timer interrupt at index: ");
    Serial.println(ArrayIndex);
    // Read the sensors
    // Readings[ArrayIndex].BatteryCurrent = read_current(ina219);
    // Readings[ArrayIndex].BatteryVoltage = read_voltage(BAT_VOLTAGE_PIN);
    // Readings[ArrayIndex].PVCurrent = read_current(ina219);
    // Readings[ArrayIndex].PVVoltage = read_voltage(PV_VOLTAGE_PIN);

    ArrayIndex++;
    if (ArrayIndex >= NUM_READINGS){
      ArrayIndex = 0;
      // Call storage function
    }
  }
}

void TimerInterrupt::set_time(int newTime){
  SetMinutes = newTime;
}