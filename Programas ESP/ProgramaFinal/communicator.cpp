#include "communicator.h"

Communicator::Communicator() : _web_server(80), _mqtt_client(_esp_client) // Changed: Initialize _web_server here
{
}

void Communicator::init()
{

    this->_setup_wifi_client();
    this->attach_interruption();
}
