// the number of the LED pin
const int PWM_Pin = 16;  // 16 corresponds to GPIO16

// setting PWM properties
const int freq = 5000; // in Hz
const int PWM_Channel = 0;
const int resolution = 8; // 8 bit resolution - value from 0 to 255
 
void setup(){
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
    delay(15);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(PWM_Channel, dutyCycle);   
    delay(15);
  }
}