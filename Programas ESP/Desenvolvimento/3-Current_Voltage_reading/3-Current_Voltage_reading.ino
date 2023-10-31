#include "Sensors.h"

Adafruit_INA219 ina219;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  ina219.setCalibration_16V_400mA();
}

void loop() {
  float current, voltage;
  current = read_current(ina219);
  voltage = read_voltage();

  Serial.print("Current: ");
  Serial.print(current);
  Serial.print("    Voltage: ");
  Serial.println(voltage);

  delay(1000);
}