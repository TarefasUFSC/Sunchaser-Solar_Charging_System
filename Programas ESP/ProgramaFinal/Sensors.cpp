#include "Sensors.h"

Sensors::Sensors(Adafruit_INA219 &bat, Adafruit_INA219 &pv)
{
  _bat_sensor = bat;
  _pv_sensor = pv;

}

void Sensors::INA_init()
{ // Initialize current sensor
  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range.
  if (!_bat_sensor.begin())
  {
    Serial.println("Failed to find Battery INA219 chip");
    while (1)
    {
      delay(10);
    }
  }
  if (!_pv_sensor.begin())
  {
    Serial.println("Failed to PV find INA219 chip");
    while (1)
    {
      delay(10);
    }
  }

  // To use a slightly lower 32V, 1A range (higher precision on amps):
  // sensor.setCalibration_32V_1A();

  // To use a slightly lower 32V, 2A range (higher precision on amps):
  // sensor.setCalibration_32V_2A();

  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //  _pv_sensor.setCalibration_16V_400mA();
  //  _bat_sensor.setCalibration_16V_400mA();

  _pv_sensor.setCalibration_32V_2A();
  _bat_sensor.setCalibration_32V_2A();
}

float Sensors::read_current(Adafruit_INA219 &sensor)
{
  float current_mA = 0;
  int mean_number = 10;

  for (int i = 0; i < mean_number; i++)
  {
    current_mA += sensor.getCurrent_mA();
  }
  current_mA = current_mA / mean_number;

  if (current_mA <= 0)
    current_mA = 0;

  return current_mA;
}

float Sensors::read_voltage(int pin)
{
  int ADC_value;
  int mean_number = 10;
  float voltage = 0, V_ref = 3.3;

  for (int i = 0; i < mean_number; i++)
  {
    ADC_value = analogRead(pin);
    float reading = (ADC_value * (V_ref / 4095));
    //    voltage +=  -0.142766377622467*reading*reading + 5.105738613083325*reading + 0.2856480236739929;
    voltage += 5 * reading;
    if (reading > 0)
    {
      voltage += 0.8;
    }
  }

  voltage = voltage / mean_number;

  return voltage;
}

float Sensors::battery_current()
{
  return read_current(_bat_sensor);
}

float Sensors::pv_current()
{
  return read_current(_pv_sensor);
}

float Sensors::battery_voltage()
{
  return read_voltage(_bat_voltage_pin);
}

float Sensors::pv_voltage()
{
  return read_voltage(_pv_voltage_pin);
}
