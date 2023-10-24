#include "Sensors.h"
#include "BatteryControl.h"

#define WIFI_LOOP 1
#define CONTROL_LOOP 2
int loop_selection = CONTROL_LOOP;

Adafruit_INA219 ina219; // Current sensor declaration

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  INA_init(ina219);
  PWM_init();
}

void loop() {
  switch(loop_selection){
    case WIFI_LOOP:
      // wifi_loop();
      break;
    case CONTROL_LOOP:
      battery_loop();
      break;
  }
}