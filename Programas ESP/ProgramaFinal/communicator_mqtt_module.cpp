#include "communicator.h"
String possible_mqtt_types[3] = {JSON_BATTERY_VOLTAGE, JSON_SOLAR_BAT_CURRENT, JSON_BAT_LOAD_CURRENT};

void Communicator::_setup_mqtt()
{
    // Initialize MQTT
    this->_mqtt_client.setServer(this->mqtt_server, this->mqtt_port);
    this->_mqtt_client.setCallback([this](char *topic, byte *payload, unsigned int length)
                                   { this->_mqtt_callback(topic, payload, length); });
}
void Communicator::_mqtt_loop()
{
    this->_mqtt_client.loop();
}

void Communicator::_mqtt_reconnect()
{
    // Loop until we're reconnected
    while (!this->_mqtt_client.connected())
    {
        if (this->check_interruption_flag())
        {
            Serial.println("Interrupção acionada, vou desistir de conectar com o broker");
            this->interrupt_handler();
            return;
        }
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (this->_mqtt_client.connect("ESP32Client"))
        {
            Serial.println("connected");
            // Subscribe to topics here
//            this->_mqtt_subscribe("your/topic");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(_mqtt_client.state());
            Serial.println(" try again in 1/2 second");
            // Wait 5 seconds before retrying
            delay(500);
        }
    }
}

void Communicator::_mqtt_subscribe(const char *topic)
{
    this->_mqtt_client.subscribe(topic);
}

bool Communicator::_mqtt_publish(const char *topic, const char *message)
{
    Serial.print("Enviando: ");
    Serial.println(message);
    Serial.print("Para: ");
    Serial.println(topic);
    // verifica se tem conexão
    if (!this->_mqtt_client.connected())
    {
        Serial.println("Não tem conexão com o broker, nem vou tentar enviar pq não vou conseguir. tente de novo mais tarde!");
        return false;
    }
    this->_mqtt_client.publish(topic, message);
    return true;
}

void Communicator::_mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    // Handle incoming messages
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

bool Communicator::send_data_to_server(Readings_Lists readings)
{
    for (int i = 0; i < NUM_READINGS; i++)
    {
        if ((readings.BatteryLoadCurrent[i].isValid!=1) || (readings.BatteryVoltage[i].isValid!=1) || (readings.PVBatteryCurrent[i].isValid!=1))
        {
          Serial.println("Os dados são invalidos");
          Serial.printf("Erro ao enviar os dados de:\nsol_bat_amp: isValid: %d, value: %f, datetime: %s\nbat_load_amp: isValid: %d, value: %f, datetime: %s\nbat_volt: isValid: %d, value: %f, datetime: %s\n",
                    readings.PVBatteryCurrent[i].isValid, readings.PVBatteryCurrent[i].value, readings.PVBatteryCurrent[i].datetime,
                    readings.BatteryLoadCurrent[i].isValid, readings.BatteryLoadCurrent[i].value, readings.BatteryLoadCurrent[i].datetime,
                    readings.BatteryVoltage[i].isValid, readings.BatteryVoltage[i].value, readings.BatteryVoltage[i].datetime);
           continue;
        }
        if(!this->send_data_to_server(readings.PVBatteryCurrent[i], readings.BatteryLoadCurrent[i], readings.BatteryVoltage[i])){
          return false;
        }
    }
    return true;
}
String convertReadingToJsonString(Reading reading)
{

    DynamicJsonDocument reading_doc(1024);
    reading_doc["value"] = reading.value;
    reading_doc["datetime"] = reading.datetime;
    String result;

    serializeJson(reading_doc, result);
    return result;
}

bool Communicator::send_data_to_server(Reading sol_bat_amp, Reading bat_load_amp, Reading bat_volt)
{ 
  this->_mqtt_reconnect();
        delay(100);
        this->_mqtt_loop();
    // cria o json no formato que o servidor espera
    // {"version":4, "<type>":<value>, "datetime":"<datetime_measurement>"}
    DynamicJsonDocument doc(2048);
    doc["version"] = 20;

    doc[JSON_SOLAR_BAT_CURRENT] = sol_bat_amp.value;
    doc[JSON_BATTERY_VOLTAGE] = bat_volt.value;
    doc[JSON_BAT_LOAD_CURRENT] = bat_load_amp.value;
    doc["datetime"] = bat_volt.datetime;
//    doc["amp"] = 1/; // isso aqui tem que tirar depois pq o codigo do eduardo ta bugado e eu não consigo remover isso do projeto

    String json;
    serializeJson(doc, json);
    Serial.println(json);
    // envia o json para o servidor
    String topic = "sensor/" + this->mac_address + "/out";
    Serial.println(topic);
    return this->_mqtt_publish(topic.c_str(), json.c_str()); // verificar qos pra verificar se conseguiu enviar ou não
}
