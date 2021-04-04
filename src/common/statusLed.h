#pragma once

#include <jled.h>

#if defined(ESP32)
    #include "WiFi.h"
    #include "esp32/IoTCore_esp32.h"
#endif

#if defined(ESP8266)
    #include "ESP8266WiFi.h"
    #include "IoTCore_esp8266.h"
#endif

enum status_led_state_t {
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

namespace status_led {
    void setup_started();
    void setup_complete();
    void uplink_started();
    void downlink_started();
    void wifi_connected();
    void wifi_disconnected();
    void mqtt_disconnected();
    void update();
    void updateStatusLed();
}  // namespace status_led
