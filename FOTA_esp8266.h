#pragma once
#include <Arduino.h>
#include "ESP8266HTTPClient.h"
#include "ESP8266httpUpdate.h"

// Both ESP8266 and ESP32 FOTA source files define this function.
void performOTAUpdate(String version);

