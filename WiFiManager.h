#pragma once

#include "Storage.h"
#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#elif ESP32
#include <WiFi.h>
#endif

#include <WiFiClientSecure.h>

enum STATES
{
    NOT_CONNECTED = 0,
    CONNECTED,
    INTERNET
};

namespace wifiman
{
    void setupWiFi();
    void reconnectWiFi(bool log);
} // namespace wifiman
