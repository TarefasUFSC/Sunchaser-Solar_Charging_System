#define VOLTAGE_PIN 35

void setup() {
  Serial.begin(115200);
}

void loop() {
  int ADC_value;
  float V_ref = 3.3;
  float voltage;
  ADC_value = analogRead(VOLTAGE_PIN);
  voltage = (ADC_value*(V_ref/4095)) * (12.5/2.51);
  
  Serial.print("ADC value: ");
  Serial.print(ADC_value);
  Serial.print("\t");
  Serial.print("Voltage: ");
  Serial.println(voltage);
  delay(500);
}
