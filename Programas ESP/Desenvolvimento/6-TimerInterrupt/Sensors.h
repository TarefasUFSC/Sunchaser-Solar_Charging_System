#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "Wire.h"
#include "Adafruit_INA219.h"

#define PV_VOLTAGE_PIN 34
#define BAT_VOLTAGE_PIN 35

void INA_init(Adafruit_INA219 &sensor);
float read_current(Adafruit_INA219 &sensor);
float read_voltage(int pin);

#endif