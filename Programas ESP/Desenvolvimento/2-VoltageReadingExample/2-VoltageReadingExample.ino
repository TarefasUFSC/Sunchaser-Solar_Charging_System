#define VOLTAGE_PIN 35

void setup() {
  Serial.begin(9600);
}

void loop() {
  int ADC_value;
  float V_ref = 3.3;
  float voltage;
  ADC_value = analogRead(VOLTAGE_PIN);
  voltage = 5*(ADC_value*(V_ref/4095) + 0.12);
  
  Serial.print("ADC value: ");
  Serial.print(ADC_value);
  Serial.print("\t");
  Serial.print("Voltage: ");
  Serial.println(voltage);
  delay(500);
}
