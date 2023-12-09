#include "communicator.h"
#include "SaveToFlash.h"
Communicator::Communicator() : _web_server(80), _mqtt_client(_esp_client) // Changed: Initialize _web_server here
{
}

void Communicator::init(SaveToFlash *files, TimeConfigurations *configs)
{
    this->_files = files;
    this->_setup_wifi_client();
    this->attach_interruption();

    this->mac_address = WiFi.macAddress();

    this->_time_configs = configs;
}
