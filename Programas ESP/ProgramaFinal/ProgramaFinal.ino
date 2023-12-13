#include "Sensors.h"
#include "BatteryControl.h"
#include "TimerInterrupt.h"
#include "SaveToFlash.h"
#include "TimeConfigurations.h"
#include "communicator.h"

Adafruit_INA219 battery_ina219(0x40);
Adafruit_INA219 pv_ina219(0x41);
Sensors sensors(battery_ina219, pv_ina219);

BatteryControl battery_controller(&sensors);

TimeConfigurations timeConfigs;
Communicator communicator;
SaveToFlash fileSystem(&timeConfigs);
TimerInterrupt timerInterrupt(&fileSystem, &communicator, &sensors, &timeConfigs);

void setup()
{
  Serial.begin(115200);

  // faz as configurações iniciais de cada objeto, dos attach dos interrupts
  sensors.INA_init();
  battery_controller.PWM_init();
  fileSystem.mountLittleFS();
  timerInterrupt.timer_init();
  
  communicator.init(&fileSystem, &timeConfigs, &battery_controller);

  // verifica o tempo da rede (fica travado aqui até ele conseguir pegar o tempo inicial pra fazer as coisas)
  // ou seja, antes daqui tando o PWM quanto o switch do Load tem que estar abertos para não dar problema, pq o esp vai ficar travado aqui
  Serial.println("Verificando o tempo da rede");
  
  communicator.setup_datetime();
  Serial.println(DateTime.toString());
  communicator.sleep(); // chama isso pra botar pra dormir -> isso aqui desliga o AP tb, então cuidado
  
}

bool envio = true; // flag pra teste, pode excluir

void loop()
{
  // battery_controller.battery_loop();
  // chama a função de verificar flags do comunicador
  communicator.interrupt_handler();
  timerInterrupt.timer_interruption();

  if (communicator.is_server)
  {
    //   Serial.println("é server");
    communicator.run_server();
    envio = true;
  }

  battery_controller.battery_loop();

//  delay(500);
}
