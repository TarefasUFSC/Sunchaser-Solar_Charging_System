#include "Sensors.h"
#include "BatteryControl.h"

Adafruit_INA219 battery_ina219(0x40);
Adafruit_INA219 pv_ina219(0x41);
Sensors sensors(battery_ina219, pv_ina219);
BatteryControl battery_controller(&sensors);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  sensors.INA_init();
  battery_controller.PWM_init();
}

void loop() {
  battery_controller.battery_loop();
}
