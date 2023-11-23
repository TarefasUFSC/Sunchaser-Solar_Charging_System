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
            this->_mqtt_subscribe("your/topic");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(_mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
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
        if (!this->send_data_to_server(JSON_BATTERY_VOLTAGE, readings.BatteryVoltage[i].value, readings.BatteryVoltage[i].datetime))
        {
            return false;
        }
        if (!this->send_data_to_server(JSON_SOLAR_BAT_CURRENT, readings.PVBatteryCurrent[i].value, readings.PVBatteryCurrent[i].datetime))
        {
            return false;
        }
        if (!this->send_data_to_server(JSON_BAT_LOAD_CURRENT, readings.BatteryLoadCurrent[i].value, readings.BatteryLoadCurrent[i].datetime))
        {
            return false;
        }
    }
    return true;
}

bool Communicator::send_data_to_server(String type, float value, String datetime_measurement)
{
    // verifica se o type ta no possible_mqtt_types
    bool type_is_valid = false;
    for (int i = 0; i < 3; i++)
    {
        if (type == possible_mqtt_types[i])
        {
            type_is_valid = true;
            break;
        }
    }
    if (!type_is_valid)
    {
        Serial.println("O type não é válido, não vou enviar");
        return false;
    }

    // cria o json no formato que o servidor espera
    // {"version":4, "<type>":<value>, "datetime":"<datetime_measurement>"}
    String json = "{\"version\":4, \"" + type + "\":" + value + ", \"datetime\":\"" + datetime_measurement + "\"}";
    Serial.println(json);
    // envia o json para o servidor
    String topic = "sensor/" + this->mac_address + "/out";
    Serial.println(topic);
    return this->_mqtt_publish(topic.c_str(), json.c_str()); // verificar qos pra verificar se conseguiu enviar ou não
}
