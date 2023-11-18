#include "communicator.h"
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

    Serial.print("Conectando ao WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectado!");
    this->is_server = false;

    this->_setup_mqtt();
}

void Communicator::check_connection()
{
    if (WiFi.status() != WL_CONNECTED && !this->is_server)
    {
        Serial.println("Desconectado do WiFi, tentando reconectar");
        this->_setup_wifi_client();
    }
}
void Communicator::get_date_from_http()
{
    if (this->is_server)
    {
        Serial.println("Você não pode fazer requisições HTTP em modo AP");
        return;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Você não pode fazer requisições HTTP sem estar conectado a uma rede");
        return;
    }

    this->_http_client.begin(_esp_client, "http://worldtimeapi.org/api/timezone/America/Sao_Paulo");
    int http_code = this->_http_client.GET();

    if (http_code > 0)
    {
        String payload = this->_http_client.getString();
        Serial.println(payload);
    }
    else
    {
        Serial.println("Falha ao fazer a requisição HTTP");
    }

    this->_http_client.end();
}
