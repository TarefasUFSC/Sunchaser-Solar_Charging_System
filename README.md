# Sunchaser-Solar_Charging_System: Controle de Carga PWM com ESP32 e Placa Fotovoltaica

## Introdução

Este projeto foi desenvolvido como parte de um trabalho para Projeto de Sistemas Ubiquos na UFSC, para o controle eficiente da carga de baterias de chumbo-ácido ou de lítio, utilizando uma placa fotovoltaica de 20W. O sistema emprega uma metodologia de modulação de largura de pulso (PWM) controlada por um ESP32 para regular a tensão e a corrente fornecida pela placa solar, assegurando uma tensão estável na bateria. Os dados de carregamento são monitorados e transmitidos para um Broker MQTT, integrando-se ao projeto do Pluviômetro existente. Adicionalmente, um aplicativo móvel fornece acesso local aos dados através de uma API REST.

O projeto é dividido em várias etapas de desenvolvimento, com testes dedicados para cada componente, seguindo princípios de orientação a objetos. Esta documentação fornece uma visão detalhada da estrutura do projeto, descrições de hardware e software, além de instruções operacionais e de diagnóstico.

## Descrição do Projeto

O sistema é projetado para operar em locais remotos, onde a estabilidade da conexão de internet pode ser intermitente. Portanto, ele armazena dados localmente e tenta transmitir informações retroativas quando uma conexão está disponível. A abordagem de design permite o acesso local aos dados do ESP32, que pode alternar entre modos cliente e servidor para facilitar a interação com o usuário por meio de um aplicativo móvel.

### Objetivos

- Manter uma tensão estável de 12V na bateria, conforme especificações técnicas do fabricante.
- Monitorar e armazenar dados de corrente e tensão para análises de longo prazo.
- Permitir o acesso local aos dados via aplicativo móvel para monitoramento e controle.
- Transmitir dados acumulados para um sistema centralizado quando conectado à internet.

Continue lendo para uma visão detalhada da implementação do projeto, incluindo instruções de montagem de hardware, configuração de software, e operação do sistema.

### Visão de Alto Nível do Sistema

![Diagrama Alto Nivel](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Documenta%C3%A7%C3%A3o/Diagramas/alto_nivel.png)

## [Hardware](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/tree/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM)

**FEITO COM PROTEUS 8.9**

### Visão Geral

O coração do projeto Sunchaser é uma solução de hardware meticulosamente projetada para o controle eficiente de carga de baterias através de energia solar. Este sistema é construído com um conjunto de placas interconectadas, cada uma desenhada para executar funções específicas dentro do mecanismo de carga.

### Compatibilidade do Proteus

Importante frisar que todo o design foi realizado utilizando o Proteus 8.9. Versões mais recentes do Proteus podem abrir os arquivos do projeto, mas há uma incompatibilidade retroativa: projetos salvos em versões mais novas do software não poderão ser reabertos na versão 8.9. Esta informação é crucial para a manutenção e futuras iterações do projeto por outras equipes.

### [Estruturação Modular do Projeto](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/tree/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/hardware_sep_by_function)

O design inicial foi acomodado na pasta `Placa de Controle/Controle de Carga PWM/hardware proteus 8.9`. Para superar desafios encontrados durante os testes, optou-se por uma abordagem modular, segmentando o hardware em diferentes funções:

- ESP com Sensores
- Controle PWM
- Controle da Carga
- Reguladores

Esta estruturação facilitou a identificação e solução de problemas, permitindo testes isolados e otimização de cada módulo.

![Modularização das Placas](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Documenta%C3%A7%C3%A3o/Diagramas/fluxograma_placas_modulo.png)

<details>
  <summary><b>Imagens e Descrições dos Circuitos</b></summary>
  Abaixo, as imagens dos circuitos correspondem às seções descritas, proporcionando uma representação visual dos componentes e sua disposição.

#### [ESP e Sensores](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/tree/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/hardware_sep_by_function/esp_and_sensors)

![Circuito da Placa do ESP e Sensores](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/imagens%20dos%20circuitos/circuito%20da%20placa%20do%20esp%20e%20sensores.png)

O circuito do ESP e sensores constitui o núcleo de processamento e coleta de dados, onde o ESP32 opera como a unidade central, gerenciando tanto a comunicação quanto o processamento dos dados de sensores.

#### [Controle PWM](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/tree/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/hardware_sep_by_function/controle_de_pot-PWM)

![Circuito Controle PWM](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/imagens%20dos%20circuitos/circuito%20placa%20de%20controle%20pwm.png)

O circuito de controle PWM manipula a tensão fornecida pela placa fotovoltaica à bateria, utilizando um MOSFET de canal P para modular a carga com precisão, baseando-se nas leituras de tensão da bateria.

#### [Controle do Switch da Carga](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/tree/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/hardware_sep_by_function/controle_de_pot-SWITCH)

![Circuito Controle do Switch da Carga](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/imagens%20dos%20circuitos/circuito%20do%20controle%20do%20switch%20da%20carga.png)

Esta parte do hardware gerencia a conectividade entre a bateria e a carga, assegurando que a transferência de energia seja feita de maneira eficaz e segura, pois o Relé é aberto caso a tensão da bateria seja muito baixa (o que danifica a vida uti dela) e caso a corrente seja alta demais para a bateria.

#### [Reguladores](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/tree/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/hardware_sep_by_function/reguladores_de_tensao)

![Circuito Reguladores](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/imagens%20dos%20circuitos/circuito%20reguladores.png)

Os reguladores de tensão são componentes cruciais do projeto, desempenhando um papel vital ao fornecer tensões DC estáveis de 5V, 9V e 12V. Esta placa reguladora é uma unidade modular, projetada para ser flexível e potencialmente integrável em outros projetos. Recebendo uma tensão de entrada variando de 12 a 20V, os reguladores asseguram a estabilização nas saídas especificadas, proporcionando uma alimentação confiável e segura para todos os componentes eletrônicos do sistema.

#### [Circuito Geral](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/tree/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/hardware%20proteus%208.9)

![Circuito Visão Geral](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Placa%20de%20Controle/Controle%20de%20Carga%20PWM/imagens%20dos%20circuitos/circuto%20placa%20all.png)

</details>

### Observações Técnicas Importantes

- A presença de uma resistência interna na simulação da bateria é vital para o comportamento adequado das tensões no circuito. A ausência desta foi contornada com a implementação de um resistor em paralelo com a fonte de alimentação durante os testes.
- A utilização do ADC1 é obrigatória para leituras analógicas, pois o ADC2 se torna inoperante quando o Wi-Fi do ESP32 está ativado. Ademais, foi observado que a entrada de 5V em qualquer pino do ADC1 pode ocasionar leituras incorretas nos outros pinos. Para resolver isso, a tensão de pulldown da interrupção foi ajustada para 3.3V.

### Futuras Melhorias e Iterações

O projeto é configurado para ser altamente escalável e adaptável. Recomenda-se que futuras equipes expandam as capacidades do sistema, incorporando novas configurações e aprimorando as especificações técnicas, como as relacionadas à bateria, para atender a novas demandas ou melhorar a eficiência do controle de carga usando MPPT.

## Configuração de Software

### Ambiente de Desenvolvimento

O código-fonte foi desenvolvido na Arduino IDE, uma plataforma de desenvolvimento que permite a escrita, compilação e upload de programas para placas compatíveis com Arduino, como o ESP32 utilizado neste projeto.

### Instalação

1. Instale a última versão da Arduino IDE a partir do [site oficial](https://www.arduino.cc/en/software).
2. Configure a Arduino IDE para suportar o ESP32 seguindo as instruções disponíveis na [documentação do ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).
3. Instale todas as bibliotecas necessárias mencionadas nos arquivos `package.json` e `yarn.lock` localizados na pasta `App Mobile/mobile`.

### Descrição dos Programas de Teste

Os programas de teste localizados em `Programas ESP/Desenvolvimento` são fundamentais para validar o funcionamento individual de cada componente do sistema. Cada subdiretório contém um exemplo de código que pode ser compilado e carregado separadamente para testar sensores, comunicação, controle PWM, entre outros.

### Programa Final

A pasta `Programas ESP/ProgramaFinal` contém o código-fonte consolidado que será executado no ESP32. Inclui a integração das classes desenvolvidas, a lógica de controle PWM e a comunicação com o Broker MQTT.

![Funcionamento Geral](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Documenta%C3%A7%C3%A3o/Diagramas/fluxograma_esp_principal.png)

# Documentação do Código

O software do projeto é composto por várias classes, cada uma responsável por uma parte específica da lógica de controle e comunicação do sistema. Abaixo estão os arquivos principais e a descrição de suas responsabilidades e operações:

<details>
  <summary><b>Classe BatteryControl</b></summary>
  
  - **Arquivos**: `BatteryControl.cpp`, `BatteryControl.h`
  - **Responsabilidade**: Controla o processo de carregamento da bateria ao modular a tensão fornecida pelo painel solar. Garante a manutenção da tensão da bateria nos níveis ideais para otimizar o carregamento e a vida útil da bateria.
  - **Métodos Principais**:
    - `PWM_init()`: Inicializa a configuração do PWM.
    - `battery_loop()`: Executa o loop principal de controle da bateria, ajustando o PWM conforme necessário.
    - `check_error(float error)`: Verifica e lida com erros no sistema de carregamento.
    - `stopCharging()`: Interrompe o processo de carregamento da bateria.
    - `bulk_stage()`, `absorption_stage()`, `float_stage()`: Gerenciam as diferentes fases do processo de carregamento.
    - `charging_control()`: Controle lógico para a regulação do carregamento da bateria.
    - `load_connection()`: Gerencia a conexão e desconexão da carga da bateria.
    - `changeDutyCycle()`: Ajusta o ciclo de trabalho do PWM para regular a tensão.

![Diagrama do Cotrole de Carga](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Documenta%C3%A7%C3%A3o/Diagramas/fluxograma_esp_controle.png)

</details>

<details>
  <summary><b>Classe Sensors</b></summary>

- **Arquivos**: `Sensors.cpp`, `Sensors.h`
- **Responsabilidade**: Realiza a leitura da corrente e tensão da bateria e do painel solar.
- **Métodos Principais**:
  - `INA_init()`: Inicializa os sensores INA219.
  - `battery_current()`, `pv_current()`: Retorna a corrente medida pela bateria e pelo sensor do painel solar.
  - `battery_voltage()`, `pv_voltage()`: Retorna a tensão medida pela bateria e pelo sensor do painel solar.
  </details>

<details>
  <summary><b>Classe TimeConfigurations</b></summary>

- **Arquivo**: `TimeConfigurations.h`
- **Responsabilidade**: Armazena as configurações de tempo relacionadas ao intervalo de leitura, tamanho máximo do cache e memória de longo prazo.
</details>

<details>
  <summary><b>Classe TimerInterrupt</b></summary>

- **Arquivos**: `TimerInterrupt.cpp`, `TimerInterrupt.h`
- **Responsabilidade**: Gerencia as interrupções do temporizador para realizar operações periódicas.
- **Métodos Principais**:

  - `timer_init()`: Inicializa o timer.
  - `timer_interruption()`: Método chamado em cada interrupção do temporizador.
  - `tryToSendCacheToServer()`: Tenta enviar dados do cache para o servidor.
  </details>
  <details>
    <summary><b>Classe SaveToFlash</b></summary>

- **Arquivos**: `SaveToFlash.cpp`, `SaveToFlash.h`
- **Responsabilidade**: Gerencia o armazenamento e recuperação de dados de leituras na memória flash.
- **Métodos Principais**:
  - `mountLittleFS()`: Monta o sistema de arquivos LittleFS.
  - `saveToCache()`, `saveToLongTerm()`: Salva as leituras no cache ou na memória de longo prazo.
  - `get_readings_from_cache()`, `get_readings_from_longterm()`: Recupera leituras do cache ou da memória de longo prazo.

![Diagrama Memoria](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Documenta%C3%A7%C3%A3o/Diagramas/diagrama_memoria_esp.png)

</details>

<details>
  <summary><b>Classe Communicator</b></summary>

- **Arquivos**: `communicator.cpp`, `communicator.h`, `communicator_client_module.cpp`, `communicator_interruption_module.cpp`, `communicator_mqtt_module.cpp`, `communicator_server_module.cpp`
- **Responsabilidade**: Esta classe gerencia toda a comunicação do ESP32, atuando como um cliente para se conectar ao Broker MQTT, e como um servidor para interagir com o aplicativo móvel. A classe também gerencia a transição entre o modo de economia de energia e operação normal e lida com interrupções externas que podem sinalizar a necessidade de mudança no estado de operação.

![Diagrama do Wifi Client](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Documenta%C3%A7%C3%A3o/Diagramas/fluxograma_esp_wifi.png)
![Diagrama do Wifi Server](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Documenta%C3%A7%C3%A3o/Diagramas/fluxograma_esp_server.png)

### Métodos Principais

- `init()`: Configura os componentes necessários e inicia a conexão Wi-Fi.
- `sleep()`: Desativa o Wi-Fi para economizar energia.
- `reconnect_client()`: Reconecta o ESP32 à rede Wi-Fi e ao broker MQTT após o período de sono.
- `attach_interruption()`: Configura a interrupção externa que permite ao ESP32 alternar entre os modos cliente e servidor.
- `check_interruption_flag()`: Verifica se uma interrupção ocorreu, indicando que o estado de operação deve ser alterado.
- `startServer()`: Inicia o servidor HTTP para comunicação local com o aplicativo móvel.
- `connectToMQTT()`: Estabelece conexão com o Broker MQTT e gerencia a reconexão se necessário.
- `handleClientRequests()`: Processa as requisições recebidas quando o ESP32 está operando como servidor HTTP.
- `_setup_mqtt()`: Configura o cliente MQTT com o servidor e porta corretos, e define a função de callback para mensagens recebidas.
- `_mqtt_loop()`: Mantém a conexão MQTT ativa e processa qualquer mensagem recebida.
- `_wifi_ap_config()`: Configura o ponto de acesso Wi-Fi quando o ESP32 está no modo servidor.
- `_stop_ap()`: Desativa o ponto de acesso Wi-Fi e o servidor HTTP quando não são mais necessários.

### Observações

- O método `sleep()` é crítico para a conservação de energia, especialmente em ambientes remotos onde a energia é limitada.
- O sistema de interrupção permite que o dispositivo mude dinamicamente entre os modos de operação sem a necessidade de reinicialização ou interação manual.
- A implementação do MQTT é robusta, com mecanismos de reconexão e callbacks para garantir a entrega de mensagens mesmo em condições de rede instáveis.
</details>

#### Pinagem

| Pino (Nome no Código) | Número do Pino | Arquivo de Declaração | Observações                                                    |
| --------------------- | -------------- | --------------------- | -------------------------------------------------------------- |
| `PWM_Pin`             | 16             | BatteryControl.h      | Utilizado para controle PWM.                                   |
| `S2_Pin`              | 26             | BatteryControl.h      | -                                                              |
| `interrupt_pin`       | 33             | communicator.h        | Não pode ser 5V para evitar problemas com ADC1.                |
| `Battery_Voltage_Pin` | 39             | Sensors.h             | Pino de leitura de tensão da bateria, deve estar no ADC1.      |
| `Solar_Voltage_Pin`   | 34             | Sensors.h             | Pino de leitura de tensão do painel solar, deve estar no ADC1. |
| `INA219_Sensor1_SCL`  | 11             | Obrigatório           | SCL do primeiro sensor INA219.                                 |
| `INA219_Sensor1_SDA`  | 10             | Obrigatório           | SDA do primeiro sensor INA219.                                 |
| `INA219_Sensor2_SCL`  | 11             | Obrigatório           | SCL do segundo sensor INA219.                                  |
| `INA219_Sensor2_SDA`  | 10             | Obrigatório           | SDA do segundo sensor INA219.                                  |

Observações Gerais:

- Os pinos de leitura analógica devem estar no ADC1 devido ao uso do Wi-Fi, que interfere com o ADC2.
- O pino `interrupt_pin` não deve receber 5V para evitar conflitos com outras leituras no ADC1.

# Operação do Programa

O ciclo de execução principal do programa é controlado pelo arquivo `ProgramaFinal.ino`, que coordena as operações das classes e gerencia o fluxo de controle do sistema. O arquivo contém a função `setup()` para inicialização e a função `loop()` para o ciclo de execução contínuo.

- Na função `setup()`, o sistema inicia conectando-se à rede Wi-Fi e sincronizando o horário para garantir que os dados sejam timestamped corretamente.
- Em seguida, os módulos de controle de carga e sensores são inicializados, preparando o sistema para começar o monitoramento e controle.
- A função `loop()` entra em um ciclo onde:
  - A lógica de controle de carga é executada para ajustar o PWM e manter a tensão da bateria.
  - As leituras dos sensores são feitas e armazenadas periodicamente no cache local.
  - O sistema tenta enviar dados acumulados para o Broker MQTT e, em caso de sucesso, transfere esses dados para a memória de longo prazo.
  - Se o switch de acesso local for ativado, o ESP32 muda para o modo servidor, permitindo que o aplicativo móvel se conecte e interaja com o sistema.

### Documentação das APIs

A comunicação com o ESP32 pode ser realizada através das seguintes APIs, disponíveis quando o ESP está em modo servidor:

| Operação | Rota      | Query Params  | Body                      | Retorno                                                                   | Observações                                           |
| -------- | --------- | ------------- | ------------------------- | ------------------------------------------------------------------------- | ----------------------------------------------------- |
| GET      | /cache    | page: Numeric | -                         | JSON: Lista de leituras do cache, total de itens, itens por página (10)   | Máximo de 10 itens por página                         |
| GET      | /ltm      | page: Numeric | -                         | JSON: Lista de leituras da memória, total de itens, itens por página (10) | Máximo de 10 itens por página                         |
| POST     | /reading  | -             | -                         | JSON: Novas medições                                                      | -                                                     |
| POST     | /settings | -             | JSON: Novas configurações | -                                                                         | O app verifica o sucesso pela resposta do status code |
| GET      | /settings | -             | -                         | JSON: Configurações atuais                                                | -                                                     |
| GET      | /check    | -             | -                         | String: "conectado!"                                                      | Utilizado para verificar a conexão com o ESP          |

![Diagrama do Aplicativo](https://github.com/TarefasUFSC/Sunchaser-Solar_Charging_System/blob/main/Documenta%C3%A7%C3%A3o/Diagramas/fluxograma_app.png)

### Diagnóstico de Problemas e Soluções

Durante o desenvolvimento, foram identificados e solucionados diversos desafios:

- **Problema de Versão do Proteus**: Limitação da versão 8.9 do Proteus para salvar e abrir arquivos sem perder a compatibilidade.
- **Comportamento Inesperado de Tensões**: Solucionado com a inclusão de uma resistência em paralelo à fonte de bancada.
- **Limitações dos ADCs do ESP32**: Ajustes nas leituras analógicas para evitar conflitos entre o ADC2 e o uso do Wi-Fi e erros causados por tensões altas nos pinos do ADC1.

### Melhorias e Manutenção

Algumas sugestões para melhorias futuras incluem:

- Inclusão de novas configurações na classe de ajustes, como especificações técnicas variáveis da bateria.
- Aumento da robustez do sistema contra falhas de conectividade.
- Interface de usuário aprimorada no aplicativo móvel para facilitar a interação e o monitoramento.

Manter a documentação atualizada e criar testes para novas funcionalidades são essenciais para a manutenção contínua do projeto.

## Estrutura de Arquivos

O projeto está organizado em diretórios que separam cada componente e fase de desenvolvimento, facilitando a navegação e compreensão do sistema como um todo. Abaixo está a árvore de arquivos com a descrição de cada diretório e arquivo-chave:

```
.
├── App Mobile
│   └── mobile
│       ├── App.js
│       ├── app.json
│       ├── babel.config.js
│       ├── package.json
│       ├── src
│       │   ├── assets
│       │   │   ├── *.png
│       │   ├── components
│       │   │   └── components.js
│       │   ├── pages
│       │   │   ├── graph.js
│       │   │   ├── home.js
│       │   │   └── settings.js
│       │   └── routes
│       │       └── routes.js
│       └── yarn.lock
├── Documentação
│   ├── Cronograma/
│   ├── Diagramas/
│   ├── Planejamento/
│   ├── Relatorios/
│   └── Revisão de Lietratura/
├── logo/
├── Placa de Controle
│   ├── Controle de Carga PWM
│   │   ├── ESP32-Pinout.jpg
│   │   ├── hardware/ -> (Antigo)
│   │   ├── hardware proteus 8.9/ -> Placa Geral com tudo incluso
│   │   ├── hardware_sep_by_function
│   │   │   ├── controle_de_pot/
│   │   │   ├── controle_de_pot-PWM/
│   │   │   ├── controle_de_pot-SWITCH/
│   │   │   ├── esp_and_sensors/
│   │   │   └── reguladores_de_tensao/
│   │   ├── imagens dos circuitos/
│   │   ├── teste controle MOSFETs/
│   ├── Cuk Converter/ -> Testes do inicio do projeto usando o conversor cuk (Não é usado mais)
├── Programas ESP
│   ├── Desenvolvimento
│   │   ├── 1-CurrentSensorExample/
│   │   ├── 2-VoltageReadingExample/
│   │   ├── 3-Current_Voltage_reading/
│   │   ├── 4-PWMControlExample/
│   │   ├── 5-BatteryControl/
│   │   ├── 6-TimerInterrupt/
│   │   ├── 7-BLE_server/
│   │   ├── 8-LittleFS/
│   │   ├── ESP-GetTime/
│   │   ├── MainProject/
│   │   └── Wifi-Server_Client/
│   ├── ProgramaFinal
│   │   ├── BatteryControl.cpp
│   │   ├── BatteryControl.h
│   │   ├── communicator_client_module.cpp
│   │   ├── communicator.cpp
│   │   ├── communicator.h
│   │   ├── communicator_interruption_module.cpp
│   │   ├── communicator_mqtt_module.cpp
│   │   ├── communicator_server_module.cpp
│   │   ├── FinalProgram.md
│   │   ├── FinalProgram.xml
│   │   ├── ProgramaFinal.ino
│   │   ├── PubSubClient.cpp
│   │   ├── PubSubClient.h
│   │   ├── SaveToFlash.cpp
│   │   ├── SaveToFlash.h
│   │   ├── Sensors.cpp
│   │   ├── Sensors.h
│   │   ├── TimeConfigurations.h
│   │   ├── TimerInterrupt.cpp
│   │   └── TimerInterrupt.h
│   └── pubsubclient-master.zip
├── README.md
└── Referencias que o professor deu no inicio
    ├── *.pdf

```
