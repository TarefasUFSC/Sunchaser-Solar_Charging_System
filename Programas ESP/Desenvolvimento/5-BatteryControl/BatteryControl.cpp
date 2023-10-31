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

extern Adafruit_INA219 ina219; // Sensor de corrente declarado no arquivo principal

void PWM_init(){
  ledcSetup(PWM_Channel, freq, resolution); // Configure PWM functionalitites
  ledcAttachPin(PWM_Pin, PWM_Channel); // Attach the channel to the GPIO to be controlled
}

void bulk_stage(){
  ledcWrite(PWM_Channel, 255); // Corrente máxima Isc, duty cycle = 1 (255)
}

void absorption_stage(){ // tentar igualar a tensão lida com AV
  float BatteryVoltage = read_voltage();
  float error = AV - BatteryVoltage;

  int dutyCycle = (int) 255*BatteryVoltage/AV; // Mapeia a tensão do banco de baterias para o duty cycle
  if(error > 0.1){
    dutyCycle ++; // Aumenta o duty cycle em 1 para aumentar a corrente de recarga
  }
  else if(error < -0.1){
    dutyCycle --; // Diminui o duty cycle em 1 para diminuir a corrente de recarga
  }

  ledcWrite(PWM_Channel, dutyCycle);  
}

// Tensão do banco de baterias é reduzida e mantida regulada no patamar da tensão de flutuação (FV)
void float_stage(){
  float BatteryVoltage = read_voltage();
  float error = FV - BatteryVoltage;

  int FV_dutyCycle = (int) 255*FV/AV;
  int dutyCycle = constrain(map(BatteryVoltage, FV, AV, FV_dutyCycle, 255), 0, 255); // Mapeia a tensão do banco de baterias para o duty cycle
  
  if( error > 0.1){
    dutyCycle ++; // Aumenta o duty cycle em 1 para aumentar a tensão de recarga
  }
  else if( error < -0.1){
    dutyCycle --; // Diminui o duty cycle em 1 para diminuir a tensão de recarga
  }
  ledcWrite(PWM_Channel, dutyCycle);
}

void charging_control(){
  float BatteryCurrent, BatteryVoltage;
  BatteryCurrent = read_current(ina219);
  BatteryVoltage = read_voltage();

  if(BatteryVoltage < AV && BatteryCurrent > TC){
    bulk_stage();
  }
  else if(BatteryVoltage >= AV && BatteryCurrent > TC){
    absorption_stage();
  }
  else if(BatteryVoltage < AV && BatteryCurrent <= TC){
    float_stage();
  }
}

void load_connection(){
  float BatteryVoltage = read_voltage();

  if(BatteryVoltage < LDV){
    digitalWrite(S2_Pin, HIGH); // Corta a alimentação da carga em S2
  }
  else if(BatteryVoltage > LRV){
    digitalWrite(S2_Pin, LOW); // Reconecta a carga em S2
  }
}

void battery_loop(){
  charging_control();
  load_connection();
}
