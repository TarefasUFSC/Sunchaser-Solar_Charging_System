// the number of the LED pin
const int PWM_Pin = 16;  // 16 corresponds to GPIO16
const int S2_Pin = 26; // 26 corresponds to GPIO26

// setting PWM properties
const int freq = 5000; // in Hz
const int PWM_Channel = 0;
const int resolution = 8; // 8 bit resolution - value from 0 to 255
 
void setup(){
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for native USB port only
  }
  pinMode(S2_Pin, OUTPUT);
  // configure PWM functionalitites
  ledcSetup(PWM_Channel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(PWM_Pin, PWM_Channel);
}
 
void loop(){
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    ledcWrite(PWM_Channel, dutyCycle);
    Serial.println(dutyCycle);
    delay(25);
  }

  Serial.println("Cortando a alimentação da carga em S2");
  digitalWrite(S2_Pin, HIGH); // Corta a alimentação da carga em S2

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(PWM_Channel, dutyCycle);   
    Serial.println(dutyCycle);
    delay(25);
  }

  Serial.println("Reconectando a carga em S2");
  digitalWrite(S2_Pin, LOW); // Reconecta a carga em S2
}