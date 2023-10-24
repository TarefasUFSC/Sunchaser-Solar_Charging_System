#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include "Wire.h"
#include "Adafruit_INA219.h"

#define VOLTAGE_PIN 35

float read_current(Adafruit_INA219 sensor);
float read_voltage();

#endif