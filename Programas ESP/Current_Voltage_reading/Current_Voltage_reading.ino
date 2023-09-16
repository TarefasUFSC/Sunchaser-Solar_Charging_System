#include "Wire.h"
#include "Adafruit_INA219.h"

#define VOLTAGE_PIN 35;

Adafruit_INA219 ina219;

float read_current(){
  float current_mA = 0;
  int mean_number = 10;

  for(int i = 0;i<mean_number;i++){
    current_mA += ina219.getCurrent_mA();
  }
  current_mA = current_mA/mean_number;

  if(current_mA<=0) current_mA = 0;

  Serial.print("Current: ");
  Serial.println(current_mA);
  return current_mA;
}

float read_voltage(){
  int ADC_value;
  float voltage,V_ref = 3.3;

  ADC_value = analogRead(VOLTAGE_PIN);
  voltage = 5*(ADC_value*(V_ref/4095) + 0.12);
  
  return voltage;
}

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
  current = read_current();
  voltage = read_voltage();

  delay(1000);
}