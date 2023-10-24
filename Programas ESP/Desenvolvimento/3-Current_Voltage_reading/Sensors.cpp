#include "Sensors.h"

float read_current(Adafruit_INA219 sensor){
  float current_mA = 0;
  int mean_number = 10;

  for(int i = 0;i<mean_number;i++){
    current_mA += sensor.getCurrent_mA();
  }
  current_mA = current_mA/mean_number;

  if(current_mA<=0) current_mA = 0;

  Serial.print("Current: ");
  Serial.println(current_mA);
  return current_mA;
}

float read_voltage(){
  int ADC_value;
  float voltage, V_ref = 3.3;

  ADC_value = analogRead(VOLTAGE_PIN);
  voltage = 5*(ADC_value*(V_ref/4095) + 0.12);
  
  return voltage;
}