#ifndef FOTA_H
#define FOTA_H

#include <Arduino.h>

// Both ESP8266 and ESP32 FOTA source files define this function.
void performOTAUpdate(String version);

#endif