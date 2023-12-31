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

#include "SaveToFlash.h"
#include "TimeConfigurations.h"
#include "BatteryControl.h"

#define JSON_BATTERY_VOLTAGE "bat_volt"
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
    SaveToFlash *_files;
    TimeConfigurations *_time_configs;
    BatteryControl *_battery_controller;

public:
    Communicator();
    void init(SaveToFlash *files, TimeConfigurations *configs, BatteryControl* battery_controller);
    void attach_interruption();
    void interrupt_handler();
    void check_flags();
    static void IRAM_ATTR on_interruption();
    void run_server();

    void setup_datetime();
    bool check_interruption_flag();
    bool isServer();
    void reconnect_client();
    void sleep();
    bool send_data_to_server(Readings_Lists readings);
    bool send_data_to_server(Reading sol_bat_amp, Reading bat_load_amp, Reading bat_volt);

private:
    void _notify_rise_edge();
    void _notify_fall_edge();

    void _reconnect_wifi();
    void _setup_wifi_client();
    void _setup_wifi_ap();
    void _stop_ap();
    void _wifi_ap_config();

    void _handle_root();
    void _handle_get_cache();
    void _handle_get_ltm();
    void _handle_get_settings();
    void _handle_post_request_new_reading();
    void _handle_put_change_settings();
    void _handle_check_connection();

    void _mqtt_reconnect();
    void _setup_mqtt();
    void _mqtt_subscribe(const char *topic);
    bool _mqtt_publish(const char *topic, const char *message);
    void _mqtt_callback(char *topic, byte *payload, unsigned int length);
    void _mqtt_loop();
    ;
};

#endif // COMMUNICATOR_H
