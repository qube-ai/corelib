#include "statusLed.h"
#include "ESP8266WiFi.h"
#include "IoTCore_esp8266.h"
#include <jled.h>

JLed hw_led = JLed(STATUS_LED_PIN).Off();
enum status_led_state_t current_state = OFF;

wl_status_t last_state = WiFi.status();

void updateStatusLed()
{
    wl_status_t current_state = WiFi.status();

    // Check whether we are connected to WIFi or not
    if (current_state != last_state)
    {
        if (current_state == WL_CONNECTED)
        {
            if (!connectedToMqtt())
            {
                Serial.println("[updateStatusLed] MQTT Disconnected");
                // status_led.mqttDisconnected();
            }
            else
            {
                status_led_wifi_connected();
                Serial.println("WiFi Connected.");
            }
        }
        else
        {
            Serial.println("WiFi Disconnected");
            status_led_wifi_disconnected();
        }

        // Update last state
        last_state = current_state;
    }
}

void status_led_setup_started()
{
    if (current_state != SETUP_STARTED)
    {
        hw_led = JLed(STATUS_LED_PIN).On();
        hw_led.Update();
        current_state = SETUP_STARTED;
        Serial.println("setupStarted() was executed");
    }
}

void status_led_setup_complete()
{
    if (current_state != SETUP_COMPLETE)
    {
        // Blink for 5 times
        hw_led.Stop();

        hw_led = JLed(STATUS_LED_PIN).DelayBefore(500).Breathe(500).Repeat(5);
        while(hw_led.Update());
        current_state = SETUP_COMPLETE;
        Serial.println("setupComplete() was executed");
    }
}

void status_led_uplink_started()
{
    if (current_state != UPLINK_STARTED)
    {
        Serial.println("uplinkStarted() was executed");
        hw_led.Stop();
        hw_led = JLed(STATUS_LED_PIN).FadeOff(500);
        while(hw_led.Update());
        current_state = UPLINK_STARTED;
        status_led_wifi_connected();
    }
    hw_led.Update();
}

// Not being used right now
void status_led_downlink_started()
{
    if (current_state != DOWNLINK_STARTED)
    {
        Serial.println("downlinkStarted() was executed");
        // Blink LED 3 times
        hw_led.Stop();
        hw_led = JLed(STATUS_LED_PIN).Breathe(500).Repeat(3);
        while(hw_led.Update());
        current_state = DOWNLINK_STARTED;
        status_led_wifi_connected();
    }
    hw_led.Update();
}

void status_led_wifi_connected()
{
    if (current_state != WIFI_CONNECTED)
    {
        Serial.println("wifiConnected() was executed");
        hw_led.Stop();
        hw_led = JLed(STATUS_LED_PIN).On().Forever();
        current_state = WIFI_CONNECTED;
    }
    hw_led.Update();
}

void status_led_wifi_disconnected()
{
    if (current_state != WIFI_DISCONNECTED)
    {
        Serial.println("wifiDisconnected() was executed");
        hw_led.Stop();
        hw_led = JLed(STATUS_LED_PIN).Blink(2000, 2000).Forever();
        current_state = WIFI_DISCONNECTED;
    }
    hw_led.Update();
}

// Not being used right now
void status_led_mqtt_disconnected()
{
    if (current_state != MQTT_DISCONNECTED)
    {
        Serial.println("mqttDisconnected() was executed");
        hw_led.Stop();
        hw_led = JLed(STATUS_LED_PIN).Blink(2000, 2000).Forever();
        current_state = MQTT_DISCONNECTED;
    }
    hw_led.Update();
}

void status_led_update() {
    hw_led.Update();
}