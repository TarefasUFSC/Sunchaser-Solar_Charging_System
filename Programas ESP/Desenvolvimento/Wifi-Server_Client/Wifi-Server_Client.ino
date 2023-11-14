#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

// Substitua com suas credenciais de WiFi
const char *connect_ssid = "RFS-S21FE";
const char *connect_password = "23011306";

// Configurações do servidor AP
const char *ap_ssid = "ESP_SUNCHASER";
const char *ap_password = NULL; // Sem senha

WebServer server(80);

// Variáveis de controle
bool isAPMode = false;
int interruptPin = 33;

void setupWiFiClient()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(connect_ssid, connect_password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
}

void setupWiFiAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ap_ssid, ap_password);
}

void handleRoot()
{
    // envia "{leituras: [1,2,3,4,5]}" em json
    server.send(200, "application/json", "{leituras: [1,2,3,4,5]}");
}

void setupServer()
{
    server.on("/leituras", handleRoot);
    server.begin();
}

void IRAM_ATTR handleInterrupt()
{
    isAPMode = !isAPMode;
    if (isAPMode)
    {
        setupWiFiAP();
        setupServer();
    }
    else
    {
        setupWiFiClient();
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, CHANGE);

    setupWiFiClient();
}

void loop()
{
    if (isAPMode)
    {
        server.handleClient();
    }
    else
    {
        // pega a hora da internet e printa na tela
        String url = "http://worldtimeapi.org/api/timezone/America/Sao_Paulo";
        HTTPClient http;
        http.begin(url);
        int httpCode = http.GET();
        if (httpCode > 0)
        {
            String payload = http.getString();
            Serial.println(payload);
        }
        else{
          Serial.println("ERRO HTTP");
        }
        http.end();
        delay(1000);
    }
}
