#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "Wire.h"
#include "Adafruit_INA219.h"

class Sensors
{
private:
    Adafruit_INA219 _pv_sensor;
    Adafruit_INA219 _bat_sensor;
    int _pv_voltage_pin = 34;
    int _bat_voltage_pin = 39;
    float read_current(Adafruit_INA219 &sensor);
    float read_voltage(int pin);

public:
    Sensors(Adafruit_INA219 &bat, Adafruit_INA219 &pv);
    void INA_init();
    float battery_current();
    float pv_current();
    float battery_voltage();
    float pv_voltage();
};

#endif
