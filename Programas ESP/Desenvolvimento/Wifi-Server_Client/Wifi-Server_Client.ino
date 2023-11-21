#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

const char *ssid = "RFS-21FE";
const char *password = "23011306";

// Configurações do servidor AP
const char *ap_ssid = "ESP_SUNCHASER";
const char *ap_password = NULL; // Sem senha

HTTPClient http;
WebServer server(80); // Servidor na porta 80

const int interruptPin = 33;
volatile bool riseFlag = false;
volatile bool fallFlag = false;
bool isServer = false;

void handleRoot()
{
    server.send(200, "application/json", "{\"leituras\":[1,2,3,4,5]}");
}
void setupWiFiAP()
{
    WiFi.mode(WIFI_AP);

    // Definir configurações de IP estático
    IPAddress local_IP(192, 168, 1, 1); // IP do ponto de acesso
    IPAddress gateway(192, 168, 1, 1);  // Gateway (geralmente o mesmo que o IP do AP)
    IPAddress subnet(255, 255, 255, 0); // Máscara de sub-rede

    // Configurar o ponto de acesso com IP estático
    if (!WiFi.softAPConfig(local_IP, gateway, subnet))
    {
        Serial.println("Falha ao configurar o IP estático");
        return;
    }

    if (WiFi.softAP(ap_ssid, ap_password))
    {
        Serial.println("Ponto de acesso configurado com sucesso");
    }
    else
    {
        Serial.println("Falha ao configurar o ponto de acesso");
        return;
    }

    Serial.print("Endereço IP do AP: ");
    Serial.println(WiFi.softAPIP());
}

void setup_wifi_server()
{
    setupWiFiAP();
    server.on("/dados", HTTP_GET, handleRoot);
    server.begin();
}

void IRAM_ATTR handleInterrupt()
{
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();

    if (interruptTime - lastInterruptTime > 200)
    {
        if (digitalRead(interruptPin) == HIGH)
        {
            riseFlag = true;
            fallFlag = false;
        }
        else
        {
            fallFlag = true;
            riseFlag = false;
        }
    }
    lastInterruptTime = interruptTime;
}

void setup_wifi_client()
{
    // Desligar o servidor e o Wi-Fi antes de mudar o modo
    server.close();
    WiFi.mode(WIFI_OFF);

    // Aguarde um pouco para que o Wi-Fi desligue completamente
    delay(1000);

    // Iniciar o Wi-Fi no modo cliente
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Conectado à rede Wi-Fi");
}

void stopAP()
{
    // Desconectar e desativar o AP
    // WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);

    // Parar o servidor
    server.close();
}

void setup()
{
    Serial.begin(9600);
    setup_wifi_client();

    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);
}

void get_time_from_http()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        http.begin("http://worldtimeapi.org/api/timezone/America/Sao_Paulo");
        int httpCode = http.GET();

        if (httpCode > 0)
        {
            String payload = http.getString();
            Serial.println(payload);
        }
        else
        {
            Serial.println("Erro na requisição HTTP");
        }

        http.end();
    }
    else
    {
        Serial.println("Erro de conexão Wi-Fi");
    }
}

void loop()
{
    if (riseFlag && !isServer)
    {
        Serial.println("Configurando como servidor");
        setup_wifi_server(); // Configurar como servidor
        riseFlag = false;
        isServer = true;
    }

    if (fallFlag && isServer)
    {
        Serial.println("Reconfigurando como cliente");
        stopAP();
        setup_wifi_client(); // Reconectar como cliente
        fallFlag = false;
        isServer = false;
    }

    if (WiFi.status() == WL_CONNECTED && !isServer)
    {
        get_time_from_http();
    }
    else
    {
        server.handleClient();
    }

    delay(1000);
}
