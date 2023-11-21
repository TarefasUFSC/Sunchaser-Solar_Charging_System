#include "Wire.h"
#include "Adafruit_INA219.h"

Adafruit_INA219 ina219;

float read_current(){
  float current_mA = 0;
  int mean_number = 100;

  for(int i = 0;i<mean_number;i++){
    current_mA += ina219.getCurrent_mA();
  }
  current_mA = current_mA/mean_number;

  if(current_mA<=0) current_mA = 0;

  Serial.print("Corrente: ");
  Serial.println(current_mA);
  return current_mA;
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
  float corrente;
  corrente = read_current();
  Serial.print("corrente: ");
  Serial.println(corrente);

  delay(1000);
}