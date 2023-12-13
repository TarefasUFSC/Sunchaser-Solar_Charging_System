/*

Bulk charge stage:
- Duty_Cycle = 1 (fluxo de energia é máximo) e S1 bloqueado
- corrente de recarga = corrente de curto-circuito I_sc do gerador fotovoltaico
- Condição para mudança de estado: a tensão nos terminais do banco de baterias alcance a tensão de absorção
(Absorption Voltage AV = 14,4 V)

Absorption stage:
- A tensão do banco de baterias é igual a AV = 14,4 V
- O controlador inicia a modulação PWM da corrente de recarga (de 1 para 0), reduzindo gradativamente seu valor
médio (I_med = valor médio da corrente terminal I_t do banco de baterias) por meio da atualização do Duty_Cycle
aplicado ao interruptor S1.
- Condição para mudança de estado: a corrente de recarga decaia abaixo do valor de corrente limiar (Treshold
current TC = 0,25 A).
- Obs: Tempo máximo de permanência no estágio de absorção (Absorption Time AT) entre 1 a 3 horas no máximo.

Float stage:
- A tensão do banco de baterias é reduzida e mantida regulada em um patamar seguro de operação, denominado por
tensão de flutuação (Float Voltage FV = 13,5 V)
- A corrente de recarga assume um valor residual (Residual Current RC)

Condição para conexão da carga em S2:
    - Tensão para desconexão da carga (LDV = 11,5 V)
    - Tensão para reconexão da carga (LRV = 12,5 V)

*/

#include "BatteryControl.h"

BatteryControl::BatteryControl(Sensors* sensors){
  read_sensors = sensors;
}

void BatteryControl::PWM_init(){
  pinMode(S2_Pin, OUTPUT);
  
  
  ledcSetup(PWM_Channel, freq, resolution); // Configure PWM functionalitites
  ledcAttachPin(PWM_Pin, PWM_Channel); // Attach the channel to the GPIO to be controlled
}
void BatteryControl::changeDutyCycle(){
  if(this->dutyCycle>255){
    this->dutyCycle = 255;
  }
   if(this->dutyCycle<0){
    this->dutyCycle = 0;
  }
  ledcWrite(PWM_Channel, this->dutyCycle); // Corrente máxima Isc, duty cycle = 1 (255)

  
  Serial.print("dutyCycle: ");
  Serial.println(this->dutyCycle);
}

void BatteryControl::bulk_stage(){
  this->dutyCycle = 255;
  this->changeDutyCycle();
}

void BatteryControl::check_error(float error){
  if(error>0){
    if(dutyCycle>0){
    this->dutyCycle --; // Diminui o duty cycle em 1 para diminuir a tensão de recarga      
    }
  }
  else if(error<0){
    this->dutyCycle ++; // Aumenta o duty cycle em 1 para aumentar a tensão de recarga
  }
  

  
  Serial.printf("Error: %f\n", error);
}

void BatteryControl::absorption_stage(){ // tentar igualar a tensão lida com AV
  float BatteryVoltage = read_sensors->battery_voltage();
  float error = BatteryVoltage - AV;

  this->check_error(error);
  

  this->changeDutyCycle();
   
}

// Tensão do banco de baterias é reduzida e mantida regulada no patamar da tensão de flutuação (FV)
void BatteryControl::float_stage(){
  float BatteryVoltage = read_sensors->battery_voltage();
  float error =  BatteryVoltage - AV;

  
  this->check_error(error);
  

  this->changeDutyCycle();
}

void BatteryControl::charging_control(){
  float BatteryCurrent, BatteryVoltage, PV_Voltage;
  BatteryCurrent = read_sensors->pv_current();
  BatteryVoltage = read_sensors->battery_voltage();

  Serial.print("Corrente PV-BAT: ");
  Serial.print(BatteryCurrent);
  Serial.print("    Tensao BAT: ");
  Serial.println(BatteryVoltage);
  Serial.print("    duty: ");
  Serial.println(this->dutyCycle);


      delay(50);


  
    if(BatteryVoltage < AV * 0.9 ||  BatteryCurrent == 0){
      Serial.println("bulk_stage");
      bulk_stage();
    }
    
    else if((BatteryVoltage <= AV* 1.1 && BatteryVoltage >= AV* 0.9 ) && (BatteryCurrent > TC ) ){
      Serial.println("absorption_stage");
      absorption_stage();
    }
    else if((BatteryVoltage <= AV* 1.1 && BatteryVoltage >= AV* 0.9 ) && (BatteryCurrent <= TC )){
      Serial.println("float_stage");
      float_stage();
    }
    
    else{
      Serial.println("Não precisa carregar");
      this->dutyCycle = 0 ;
      this->changeDutyCycle();
    }
}

void BatteryControl::load_connection(){
  float BatteryVoltage,PV_Voltage,BatLoadCurrent;
  BatteryVoltage = read_sensors->battery_voltage();
  BatLoadCurrent = read_sensors->battery_current();
  PV_Voltage = read_sensors->pv_voltage();
  Serial.print("    Tensao PV: ");
  Serial.println(PV_Voltage);
  
  Serial.print("    Corrente BAT-LOAD: ");
  Serial.print(BatLoadCurrent);
  

  if(BatteryVoltage < LDV || BatLoadCurrent >= LDC){
    Serial.println("DESCONECTANDO CARGA!");
    digitalWrite(S2_Pin, HIGH); // Corta a alimentação da carga em S2
    delay(1000);
  }
  else if(BatteryVoltage > LRV){
    digitalWrite(S2_Pin, LOW); // Reconecta a carga em S2
  }
}

void BatteryControl::battery_loop(){
  if(!started){
    this->dutyCycle = 255;
    this->changeDutyCycle();
    delay(500);
    started = true;
  }
  charging_control();
  load_connection();
}
