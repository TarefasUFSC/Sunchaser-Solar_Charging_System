

class Sensors
{
};

class BatteryController
{
};

class Comunicator
{
};

class FileSystem
{
};

class TimerInterrupt
{
};

// Cria as instancias globais das classes

void setup()
{
    // faz as configurações iniciais de cada objeto, dos attach dos interrupts

    // verifica o tempo da rede (fica travado aqui até ele conseguir pegar o tempo inicial pra fazer as coisas)
    // ou seja, antes daqui tando o PWM quanto o switch do Load tem que estar abertos para não dar problema, pq o esp vai ficar travado aqui
}

void loop()
{

    // chama a função de verificar flags do comunicador

    // if( not comunidaor.isServer)
    // verifica o counter do TimerInterrupt e faz as chamadas
    // timer_interruption(fileSystem, Sensors, comunicator) // -> faz as verificações de tempo e levanta as flags de processos

    // chama a função de controle da bateria
    // battery_loop(sensors)
}
