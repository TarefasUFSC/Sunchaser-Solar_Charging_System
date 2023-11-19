#include "communicator.h"

void Communicator::_setup_mqtt()
{
    // Initialize MQTT
    this->_mqtt_client.setServer(this->mqtt_server, this->mqtt_port);
    this->_mqtt_client.setCallback([this](char *topic, byte *payload, unsigned int length)
                                   { this->mqtt_callback(topic, payload, length); });
}
void Communicator::mqtt_loop()
{
    this->_mqtt_client.loop();
}

void Communicator::mqtt_reconnect()
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
            this->mqtt_subscribe("your/topic");
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

void Communicator::mqtt_subscribe(const char *topic)
{
    this->_mqtt_client.subscribe(topic);
}

void Communicator::mqtt_publish(const char *topic, const char *message)
{
    this->_mqtt_client.publish(topic, message);
}

void Communicator::mqtt_callback(char *topic, byte *payload, unsigned int length)
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
