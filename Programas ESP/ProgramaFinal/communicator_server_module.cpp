#include "communicator.h"
void Communicator::_stop_ap()
{
    // WiFi.softAPdisconnect(true); // por algum motivo deixar isso faz com que ele não feche o server e a rede continua aparecendo
    WiFi.mode(WIFI_OFF);
    this->_web_server.close();
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

    if (WiFi.softAP(this->_ssid_wifi_ap, this->_password_wifi_ap))
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
    // printa o endereço mac
    Serial.print("MAC do AP: ");
    Serial.println(WiFi.softAPmacAddress());
}

void Communicator::_handle_root()
{
    // retorna uma pagina web com as informações do projeto
    // Nome: Sunchaser - COntrole de Carga de Bateria de Chumbo com Placa solares
    // Membros: Lucas Eduardo, Manoella ROckembach e Rodrigo Ferraz
    Serial.println("ROOT");
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

    this->_web_server.begin();
    this->is_server = true;
}

void Communicator::run_server()
{
    this->_web_server.handleClient();
}