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
        bool started = false;

        // S2 load pin configuration
        const int S2_Pin = 26; // 26 corresponds to GPIO26

        // constants definitions (pontos de ajuste)
        const float AV = 11.5;  // Absorption Voltage (14.4 V) - para a de teste é 12.6 (litio)
        const float TC = 22;  // Threshold Current in mA (250mA)
        const float FV = 10;  // Float Voltage (13.5 V)
        const float LDV = 9; // Load Disconnect Voltage
        const float LRV = 11; // Load Reconnect Voltage

        Sensors* read_sensors;

        void bulk_stage();
        void absorption_stage();
        void float_stage();
        void charging_control();
        void load_connection();
        void changeDutyCycle();
    public:
        BatteryControl(Sensors* sensors);
        void PWM_init();
        void battery_loop();
        void check_error(float error);
};

#endif
