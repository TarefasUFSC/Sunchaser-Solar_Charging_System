#include "Sensors.h"

// Setting PWM properties
const int PWM_Pin = 16;  // 16 corresponds to GPIO16
const int freq = 5000; // in Hz
const int PWM_Channel = 0;
const int resolution = 8; // 8 bit resolution - value from 0 to 255

Adafruit_INA219 ina219; // Current sensor

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }

  // Initialize current sensor
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  ina219.setCalibration_16V_400mA(); // Current sensor calibration

  ledcSetup(PWM_Channel, freq, resolution); // Configure PWM functionalitites
  
  ledcAttachPin(PWM_Pin, PWM_Channel); // Attach the channel to the GPIO to be controlled
}

void loop() {
  float current, voltage;
  current = read_current(ina219);
  voltage = read_voltage();

  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    ledcWrite(PWM_Channel, dutyCycle);
    delay(15);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(PWM_Channel, dutyCycle);   
    delay(15);
  }

  delay(1000);
}