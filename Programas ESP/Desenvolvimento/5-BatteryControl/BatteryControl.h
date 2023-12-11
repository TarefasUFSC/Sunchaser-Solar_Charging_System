#ifndef BATTERYCONTROL_H
#define BATTERYCONTROL_H

#include <Arduino.h>
#include "Sensors.h"

class BatteryControl {
    private:
        // setting PWM properties
        const int PWM_Pin = 16;  // 16 corresponds to GPIO16
        const int freq = 50000; // in Hz
        const int PWM_Channel = 0;
        const int resolution = 8; // 8 bit resolution - value from 0 to 255
        int dutyCycle = 0;

        // S2 load pin configuration
        const int S2_Pin = 26; // 26 corresponds to GPIO26

        // constants definitions (pontos de ajuste)
        const float AV = 14;  // Absorption Voltage (14.4 V)
        const float TC = 10.0;  // Threshold Current in mA (250mA)
        const float FV = 12.8;  // Float Voltage (13.5 V)
        const float LDV = 11.5; // Load Disconnect Voltage
        const float LRV = 12.5; // Load Reconnect Voltage

        Sensors* read_sensors;

        void bulk_stage();
        void absorption_stage();
        void float_stage();
        void charging_control();
        void load_connection();
    public:
        BatteryControl(Sensors* sensors);
        void PWM_init();
        void battery_loop();
};

#endif