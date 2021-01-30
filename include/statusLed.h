#pragma once

#include <jled.h>
#define STATUS_LED_PIN 12

enum status_led_state_t
    {
        OFF,
        SETUP_STARTED,
        SETUP_COMPLETE,
        UPLINK_STARTED,
        DOWNLINK_STARTED,
        WIFI_CONNECTED,
        WIFI_DISCONNECTED,
        MQTT_DISCONNECTED
    };

extern enum status_led_state_t current_state;
extern JLed hw_led;

void status_led_setup_started();
void status_led_setup_complete();
void status_led_uplink_started();
void status_led_downlink_started();
void status_led_wifi_connected();
void status_led_wifi_disconnected();
void status_led_mqtt_disconnected();
void status_led_update();
void updateStatusLed();