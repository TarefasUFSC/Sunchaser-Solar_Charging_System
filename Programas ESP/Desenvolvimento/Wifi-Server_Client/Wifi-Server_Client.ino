#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "RFS-21FE";
const char *password = "23011306";

HTTPClient http;

const int interruptPin = 33; // Pino de interrupção
volatile bool riseFlag = false;
volatile bool fallFlag = false;

void IRAM_ATTR handleInterrupt()
{
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();

    // Somente aceita a interrupção se o tempo decorrido desde a última for maior que um limite (por exemplo, 200ms)
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
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Conectado à rede Wi-Fi");
}

void setup()
{
    Serial.begin(115200);
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
    if (riseFlag)
    {
        Serial.println("Interrupção por borda de subida detectada");
        riseFlag = false;
    }

    if (fallFlag)
    {
        Serial.println("Interrupção por borda de descida detectada");
        fallFlag = false;
    }

    get_time_from_http();
    delay(1000); // Esperar 1 segundos para a próxima requisição
}
