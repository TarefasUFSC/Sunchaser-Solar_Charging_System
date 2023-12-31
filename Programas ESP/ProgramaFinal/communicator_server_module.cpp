#include "communicator.h"
#include "SaveToFlash.h"

#include <string>

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

void Communicator::_setup_wifi_ap()
{
    this->_wifi_ap_config();

    // Inicia o servidor
    // root
    this->_web_server.on("/", HTTP_GET, std::bind(&Communicator::_handle_root, this));
    // cache
    this->_web_server.on("/cache", HTTP_GET, std::bind(&Communicator::_handle_get_cache, this));
    // long term memmory
    this->_web_server.on("/ltm", HTTP_GET, std::bind(&Communicator::_handle_get_ltm, this));
    // get settings
    this->_web_server.on("/settings", HTTP_GET, std::bind(&Communicator::_handle_get_settings, this));
    // post new reading
    this->_web_server.on("/reading", HTTP_POST, std::bind(&Communicator::_handle_post_request_new_reading, this));
    // put change settings
    this->_web_server.on("/settings", HTTP_PUT, std::bind(&Communicator::_handle_put_change_settings, this));
    // check connection
    this->_web_server.on("/check", HTTP_GET, std::bind(&Communicator::_handle_check_connection, this));

    this->_web_server.begin();
    this->is_server = true;
}

void Communicator::run_server()
{
    this->_web_server.handleClient();
}

void Communicator::_handle_check_connection()
{
    this->_web_server.send(200, "text/plain", "conectado!");
}

void Communicator::_handle_get_cache()
{
    String page = "";
    if (this->_web_server.hasArg("page"))
    {                                         // Verifica se o argumento "page" existe
        page = this->_web_server.arg("page"); // Obtém o valor do argumento "page"

        DynamicJsonDocument doc(2048);
        doc["page"] = page;
        doc["total"] = this->_files->getNCacheSaves();
        doc["qtd_per_page"] = NUM_READINGS;

        Readings_Lists readings = this->_files->get_readings_from_cache(atoi(page.c_str()));
        for (int i = 0; i < NUM_READINGS; i++)
        {
            if (readings.BatteryLoadCurrent[i].isValid && readings.BatteryVoltage[i].isValid && readings.PVBatteryCurrent[i].isValid)
            {

                Serial.print(i);
                doc[JSON_BAT_LOAD_CURRENT][i]["datetime"] = readings.BatteryLoadCurrent[i].datetime;
                doc[JSON_BAT_LOAD_CURRENT][i]["value"] = readings.BatteryLoadCurrent[i].value;

                doc[JSON_BATTERY_VOLTAGE][i]["datetime"] = readings.BatteryVoltage[i].datetime;
                doc[JSON_BATTERY_VOLTAGE][i]["value"] = readings.BatteryVoltage[i].value;

                doc[JSON_SOLAR_BAT_CURRENT][i]["datetime"] = readings.PVBatteryCurrent[i].datetime;
                doc[JSON_SOLAR_BAT_CURRENT][i]["value"] = readings.PVBatteryCurrent[i].value;
            }
        }

        Serial.println("enviado os dados do chache");
        String return_data;
        serializeJson(doc, return_data);
        this->_web_server.send(200, "application/json", return_data);
        return;
    }
    else
    {
        this->_web_server.send(400, "text/plain", "400: Erro na requisição: argumento 'page' não encontrado");
    }
}

void Communicator::_handle_get_ltm()
{
    String page = "";
    if (this->_web_server.hasArg("page"))
    {                                         // Verifica se o argumento "page" existe
        page = this->_web_server.arg("page"); // Obtém o valor do argumento "page"

        DynamicJsonDocument doc(2048);
        doc["page"] = page;
        doc["total"] = this->_files->getNLongTermSaves();
        doc["qtd_per_page"] = NUM_READINGS;

        Readings_Lists readings = this->_files->get_readings_from_longterm(atoi(page.c_str()));

        for (int i = 0; i < 5; i++)
        {
            doc[JSON_BAT_LOAD_CURRENT][i]["datetime"] = readings.BatteryLoadCurrent[i].datetime;
            doc[JSON_BAT_LOAD_CURRENT][i]["value"] = readings.BatteryLoadCurrent[i].value;
            doc[JSON_BATTERY_VOLTAGE][i]["datetime"] = readings.BatteryVoltage[i].datetime;
            doc[JSON_BATTERY_VOLTAGE][i]["value"] = readings.BatteryVoltage[i].value;
            doc[JSON_SOLAR_BAT_CURRENT][i]["datetime"] = readings.PVBatteryCurrent[i].datetime;
            doc[JSON_SOLAR_BAT_CURRENT][i]["value"] = readings.PVBatteryCurrent[i].value;
        }

        String return_data;
        serializeJson(doc, return_data);
        this->_web_server.send(200, "application/json", return_data);

        return;
    }
    else
    {
        this->_web_server.send(400, "text/plain", "400: Erro na requisição: argumento 'page' não encontrado");
    }
}
void Communicator::_handle_get_settings()
{
    // TODO - SETTINGS: ver com a manu como que eu vou pegar essas informações
    DynamicJsonDocument doc(1024);
    doc["readingInterval"] = this->_time_configs->get_ReadingInterval();
    doc["cacheMaxSize"] = this->_time_configs->get_CacheMaxSize();
    doc["ltmMaxSize"] = this->_time_configs->get_LTMMaxSize();
    String output;
    serializeJson(doc, output);
    this->_web_server.send(200, "application/json", output);
}
void Communicator::_handle_post_request_new_reading()
{
    // pede uma nova leitura ubindo alguma flag provavelmnete.
    // TODO - Falar com a manu e perguntar como que eu vou avisar ela que eu tenho que pedir uma nova leitura e como que eu vou pegar esse dado aqui

    this->_web_server.send(200, "application/json", "{\"leitura\":1}");
}
void Communicator::_handle_put_change_settings()
{
    if (this->_web_server.hasArg("plain") == false)
    { // Verifica se há dados no corpo
        this->_web_server.send(400, "text/plain", "Body not received");
        return;
    }

    String message = this->_web_server.arg("plain"); // Obtém o corpo da requisição

    // parse do json recebido
    // config1, config2, config3, config4, config5
    // TODO - VERIFICAR COM A MANU COMO QUE ELA TA SALVANDO ESSAS CONFIGS

    // pega a configo do json
    DynamicJsonDocument settings_json_doc(1024);
    deserializeJson(settings_json_doc, message);
    JsonObject obj = settings_json_doc.as<JsonObject>();

    Serial.println(obj["readingInterval"].as<String>());
    this->_time_configs->set_ReadingInterval(obj["readingInterval"].as<int>());
    this->_time_configs->set_CacheMaxSize(obj["cacheMaxSize"].as<int>());
    this->_time_configs->set_LTMaxSize(obj["ltmMaxSize"].as<int>());

    this->_web_server.send(200, "text/plain", "OK");
}
