#include "communicator.h"

Communicator::Communicator() : _web_server(80) // Changed: Initialize _web_server here
{
}

void Communicator::init()
{

    this->_setup_wifi_client();
       pinMode(this->interrupt_pin, INPUT);

    this->_last_switch_state = digitalRead(this->interrupt_pin);
    // this->attach_interruption();
}


void Communicator::_stop_ap()
{
    // WiFi.softAPdisconnect(true); // por algum motivo deixar isso faz com que ele não feche o server e a rede continua aparecendo
    WiFi.mode(WIFI_OFF);
    this->_web_server.close();
    this->is_server = false;
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

    Serial.print("Conectando ao WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectado!");
    this->is_server = false;
}
void Communicator::_wifi_ap_config()
{
    WiFi.mode(WIFI_AP);

    // Define IP estático
    IPAddress ip(192, 168, 1, 1);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    // Configura o AP
    if (!WiFi.softAPConfig(ip, gateway, subnet))
    {
        Serial.println("Falha ao configurar o AP");
        return;
    }

    if (WiFi.softAP(_ssid_wifi_ap, _password_wifi_ap))
    {
        Serial.println("AP configurado com sucesso");
    }
    else
    {
        Serial.println("Falha ao configurar o AP");
        return;
    }

    Serial.print("IP do AP: ");
    Serial.println(WiFi.softAPIP());
}

void Communicator::_handle_root()
{
    // retorna uma pagina web com as informações do projeto
    // Nome: Sunchaser - COntrole de Carga de Bateria de Chumbo com Placa solares
    // Membros: Lucas Eduardo, Manoella ROckembach e Rodrigo Ferraz

    this->_web_server.send(200, "text/html", "<h1>Sunchaser</h1><p>Controle de Carga de Bateria de Chumbo com Placa solares</p><p>Membros: Lucas Eduardo, Manoella ROckembach e Rodrigo Ferraz</p>");
}

void Communicator::_handle_get_cache()
{
    this->_web_server.send(200, "application/json", "{\"leituras\":[1,2,3,4,5]}");
}

void Communicator::_setup_wifi_ap()
{
    this->_wifi_ap_config();

    // Inicia o servidor
    // root
    this->_web_server.on("/", HTTP_GET, std::bind(&Communicator::_handle_root, this));
    // cache
    this->_web_server.on("/cache", HTTP_GET, std::bind(&Communicator::_handle_get_cache, this));

    this->is_server = true;
}
void Communicator::_notify_rise_edge()
{
    Serial.println("Rise");
    this->_rise_flag = true;
    this->_fall_flag = false;
}
void Communicator::_notify_fall_edge()
{
    Serial.println("Fall");
    this->_fall_flag = true;
    this->_rise_flag = false;
}
void Communicator::verify_switch()
{
    bool sw_state = digitalRead(this->interrupt_pin);
    if (sw_state != this->_last_switch_state)
    {
        this->_last_switch_state = sw_state;
        if (sw_state)
        {
            this->_notify_rise_edge();
        }
        else
        {
            this->_notify_fall_edge();
        }
    }

    this->check_flags();
}


void Communicator::check_flags()
{
    if (this->_rise_flag && !this->is_server)
    {
        Serial.println("Configurando o AP");
        this->_setup_wifi_ap();
        this->_rise_flag = false;
    }
    else if (this->_fall_flag && this->is_server)
    {
        Serial.println("Desligando o AP e configurando o Station");
        this->_setup_wifi_client();
        this->_fall_flag = false;
    }
}

void Communicator::run_server()
{
    if (WiFi.status() == WL_CONNECTED && this->is_server)
    {
        this->_web_server.handleClient();
    }
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

    this->_http_client.begin("http://worldtimeapi.org/api/timezone/America/Sao_Paulo");
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
