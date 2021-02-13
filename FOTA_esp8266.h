#pragma once

#include <Arduino.h>

#include "ESP8266WiFi.h"
#include "ESP8266WiFiMulti.h"

#include "ESP8266HTTPClient.h"
#include "ESP8266httpUpdate.h"

#ifndef APSSID
#define APSSID "Curiosity"
#define APPSK "duckduckgo123"
#endif

// Both ESP8266 and ESP32 FOTA source files define this function.
namespace fota {
    void performOTAUpdate(String version);
}


