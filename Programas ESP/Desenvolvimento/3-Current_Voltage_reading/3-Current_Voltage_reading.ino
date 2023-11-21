#include "Sensors.h"

/* calculated I2C address: 0 = GND, 1 = V+
 * The address is controlled by the A0 and A1 inputs on the INA219:
 *
 * Calculated address: b100ABCD
 * A0 controls C and D: GND = 00, V+ = 01, SDA = 10, SCL = 11
 * A1 controls A and B: GND = 00, V+ = 01, SDA = 10, SCL = 11
 *
 * E.g. if A0 is tied to ground and A1 is tied to V+,
 * the resulting address is b1000100 = 0x44
 */
Adafruit_INA219 battery_ina219(0x40);
Adafruit_INA219 pv_ina219(0x44);
Sensors sensors(battery_ina219, pv_ina219);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  sensors.INA_init();
}

void loop() {
  float bat_current, pv_current, bat_voltage, pv_voltage;
  bat_current = sensors.battery_current();
  pv_current = sensors.pv_current();
  bat_voltage = sensors.battery_voltage();
  pv_voltage = sensors.pv_voltage();

  Serial.print("Battery current: ");
  Serial.print(bat_current);
  Serial.print("    PV current: ");
  Serial.print(pv_current);
  Serial.print("    Battery voltage: ");
  Serial.print(bat_voltage);
  Serial.print("    PV voltage: ");
  Serial.println(pv_voltage);

  delay(1000);
}