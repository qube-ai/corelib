#ifndef FOTA_H
#define FOTA_H

#include <Arduino.h>
#include <Update.h>
#include <WiFi.h>

#include "FOTA_esp32.h"

// Both ESP8266 and ESP32 FOTA source files define this function.
namespace fota {
    void performOTAUpdate(String version);
}

#endif