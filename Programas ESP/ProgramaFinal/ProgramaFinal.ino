#include "communicator.h"
// class Sensors
// {
// };

// class BatteryController
// {
// };

// class FileSystem
// {
// };

// class TimerInterrupt
// {
// };

// Cria as instancias globais das classes
Communicator communicator;

void setup()
{
    Serial.begin(9600);

    // faz as configurações iniciais de cada objeto, dos attach dos interrupts
    communicator.init();

    // verifica o tempo da rede (fica travado aqui até ele conseguir pegar o tempo inicial pra fazer as coisas)
    // ou seja, antes daqui tando o PWM quanto o switch do Load tem que estar abertos para não dar problema, pq o esp vai ficar travado aqui
    communicator.get_date_from_http();
}

void loop()
{

      bool pinState = digitalRead(communicator.interrupt_pin);
      Serial.println(pinState ? "High" : "Low");

    // chama a função de verificar flags do comunicador
    communicator.verify_switch();

    if (communicator.is_server)
    {
        communicator.run_server();
    }

    // if( not comunidaor.isServer)
    // verifica o counter do TimerInterrupt e faz as chamadas
    // timer_interruption(fileSystem, Sensors, comunicator) // -> faz as verificações de tempo e levanta as flags de processos

    // chama a função de controle da bateria
    // battery_loop(sensors)
    delay(500);
}
