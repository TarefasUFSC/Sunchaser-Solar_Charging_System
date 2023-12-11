#include "Sensors.h"

extern Adafruit_INA219 ina219; // Sensor de corrente declarado no arquivo principal

void INA_init(Adafruit_INA219 &sensor){ // Initialize current sensor
  if (! sensor.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  sensor.setCalibration_16V_400mA(); // Current sensor calibration
}

float read_current(Adafruit_INA219 &sensor){
  float current_mA = 0;
  int mean_number = 10;

  for(int i = 0;i<mean_number;i++){
    current_mA += sensor.getCurrent_mA();
  }
  current_mA = current_mA/mean_number;

  if(current_mA<=0) current_mA = 0;

  return current_mA;
}

float read_voltage(int pin){
  int ADC_value;
  float voltage, V_ref = 3.3;

  ADC_value = analogRead(pin);
  voltage = 5*(ADC_value*(V_ref/4095) + 0.12);
  
  return voltage;
}