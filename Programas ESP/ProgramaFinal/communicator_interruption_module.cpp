#include "communicator.h"

// volatile SemaphoreHandle_t Communicator::_interruption_semaphore = xSemaphoreCre/ateBinary();
volatile bool interruption_flag = false;
volatile unsigned long last_interruption_time = 0;

void Communicator::attach_interruption()
{
    pinMode(this->interrupt_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(this->interrupt_pin), &Communicator::on_interruption, CHANGE);
}
bool Communicator::check_interruption_flag()
{
    return interruption_flag;
}
void Communicator::_notify_rise_edge()
{
    Serial.println("Rise");
    this->_rise_flag = true;
    this->_fall_flag = false;
}
void Communicator::_notify_fall_edge()
{
    Serial.println("Fall");
    this->_fall_flag = true;
    this->_rise_flag = false;
}

void Communicator::interrupt_handler()
{
    if (this->check_interruption_flag())
    {
        interruption_flag = false;
        unsigned long interruption_time = millis();
        if (interruption_time - last_interruption_time > 200)
        {
            last_interruption_time = interruption_time;
            if (digitalRead(this->interrupt_pin) == HIGH)
            {
                this->_notify_rise_edge();
            }
            else
            {
                this->_notify_fall_edge();
            }
        }
    }

    this->check_flags();
}

void IRAM_ATTR Communicator::on_interruption()
{
    interruption_flag = true;
}

void Communicator::check_flags()
{
    if (this->_rise_flag && !this->is_server)
    {
        Serial.println("Configurando o AP");
        this->_setup_wifi_ap();
        this->_rise_flag = false;
    }
    else if (this->_fall_flag && this->is_server)
    {
        Serial.println("Desligando o AP e configurando o Station");
        this->_setup_wifi_client();
        this->_fall_flag = false;
    }
}
