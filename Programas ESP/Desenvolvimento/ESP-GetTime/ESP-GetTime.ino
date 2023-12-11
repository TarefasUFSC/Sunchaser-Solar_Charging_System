#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "RFS-21FE";
const char *password = "23011306";

HTTPClient http;

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

    // Conectar à rede Wi-Fi
    setup_wifi_client();
}

void get_time_from_http()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        http.begin("http://worldtimeapi.org/api/timezone/America/Sao_Paulo"); // Iniciar conexão com a URL
        int httpCode = http.GET();                                            // Fazer requisição HTTP GET

        if (httpCode > 0)
        {                                      // Verificar se a resposta é válida
            String payload = http.getString(); // Pegar a resposta
            Serial.println(payload);
        }
        else
        {
            Serial.println("Erro na requisição HTTP");
        }

        http.end(); // Encerrar conexão
    }
    else
    {
        Serial.println("Erro de conexão Wi-Fi");
    }
}

void loop()
{

    get_time_from_http();

    delay(10000); // Esperar 10 segundos para a próxima requisição
}
