#include "communicator.h"
void Communicator::sleep()
{
  Serial.println("Indo dormir");
    // desliga o wifi para economizar energia
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}
void Communicator::reconnect_client()
{
    if (this->is_server)
    {
        Serial.println("Vc é um server, não vou reconectar o client");
    }
    else
    {
        this->_setup_wifi_client();
        delay(100);
        this->_mqtt_reconnect();
        delay(100);
        this->_mqtt_loop();
    }
}
void Communicator::_reconnect_wifi()
{
  delay(1000);
    if (this->is_server)
    {
        Serial.println("Vc é um server, não vou reconectar o wifi");
    }
    else
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            return;
        }
        WiFi.begin(_ssid_wifi_to_connect, _password_wifi_to_connect);
        delay(500);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("\nConectado!");

        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        // mac
        Serial.print("MAC: ");
        Serial.println(WiFi.macAddress());

        this->mac_address = WiFi.macAddress();
    }
}
void Communicator::_setup_wifi_client()
{
    if (this->is_server)
    {
        this->_stop_ap();
    }

    // Aguarda para que o WiFi esteja pronto
    delay(1000);

    // Inicia o WiFi em modo Station
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid_wifi_to_connect, _password_wifi_to_connect);

    this->_reconnect_wifi();

    this->is_server = false;

    this->_setup_mqtt();
}

void Communicator::setup_datetime()
{
    // Setup this after WiFi is connected
    DateTime.setServer("pool.ntp.org"); // Usando um servidor NTP genérico
    DateTime.setTimeZone("UTC+3");      // Configurando para o fuso horário de Brasília (os caras inverteram + com - na lib)
    DateTime.begin();

    while (!DateTime.isTimeValid())
    {
        Serial.println("Falha ao obter o horário do servidor NTP.");
    }
    Serial.printf("Data e hora atuais: %s\n", DateTime.toISOString().c_str());
    Serial.printf("Timestamp Unix: %ld\n", DateTime.now());
}
