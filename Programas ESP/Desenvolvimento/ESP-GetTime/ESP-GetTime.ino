#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Substitua com as credenciais da sua rede Wi-Fi
const char *ssid = "RFS-21FE";
const char *password = "23011306";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup()
{
    Serial.begin(115200);

    // Conectar à rede Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Conectado à rede Wi-Fi");

    // Iniciar cliente NTP
    timeClient.begin();
    // Definir fuso horário em horas (exemplo para GMT+1)
    // Altere conforme necessário
    timeClient.setTimeOffset(3600);
}

void loop()
{
    timeClient.update();

    // Obter a hora formatada
    Serial.println(timeClient.getFormattedTime());

    delay(1000);
}
