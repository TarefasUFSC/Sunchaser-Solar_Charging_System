#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include "PubSubClient.h"
#include <ArduinoJson.h>
#include <DateTime.h>
#include <ESPDateTime.h>

#define JSON_BATTERY_VOLTAGE_TYPE "bat_volt"
#define JSON_SOLAR_BAT_CURRENT "sol_bat_amp"
#define JSON_BAT_LOAD_CURRENT "bat_load_amp"

class Communicator
{
public:
    volatile bool is_server = false;
    const int interrupt_pin = 33;
    String mac_address;

private:
    const char *_ssid_wifi_to_connect = "RFS-S21FE";
    const char *_password_wifi_to_connect = "23011306";
    HTTPClient _http_client;

    const char *_ssid_wifi_ap = "ESP32_SUNCHASER";
    const char *_password_wifi_ap = NULL;
    WebServer _web_server;

    volatile bool _rise_flag = false;
    volatile bool _fall_flag = false;

    const char *mqtt_server = "150.162.235.160";
    const int mqtt_port = 8004;
    WiFiClient _esp_client;
    PubSubClient _mqtt_client;

public:
    Communicator();
    void init();
    void attach_interruption();
    void interrupt_handler();
    void check_flags();
    static void IRAM_ATTR on_interruption();
    void run_server();
    void check_connection();
    void mqtt_reconnect();
    void mqtt_subscribe(const char *topic);
    bool mqtt_publish(const char *topic, const char *message);
    void mqtt_callback(char *topic, byte *payload, unsigned int length);
    void setup_datetime();
    void mqtt_loop();
    bool check_interruption_flag();
    void reconnect_wifi();
    bool send_data_to_server(String type, float value, String datetime_measurement);

private:
    void _setup_mqtt();
    void _notify_rise_edge();
    void _notify_fall_edge();
    void _setup_wifi_client();
    void _setup_wifi_ap();
    void _stop_ap();
    void _wifi_ap_config();
    void _handle_root();
    void _handle_get_cache();
    ;
};

#endif // COMMUNICATOR_H
