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
    Serial.println("Verificando o tempo da rede");
    communicator.setup_datetime();
    Serial.println(DateTime.toString());
}

void loop()
{

    // chama a função de verificar flags do comunicador
    communicator.interrupt_handler();

    if (communicator.is_server)
    {
        //   Serial.println("é server");
        communicator.run_server();
    }
    else
    {
        communicator.mqtt_reconnect();
        communicator.mqtt_loop();
    }

    // if( not comunidaor.isServer)
    // verifica o counter do TimerInterrupt e faz as chamadas
    // timer_interruption(fileSystem, Sensors, comunicator) // -> faz as verificações de tempo e levanta as flags de processos

    // chama a função de controle da bateria
    // battery_loop(sensors)
    delay(500);
}
