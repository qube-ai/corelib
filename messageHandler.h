#pragma once

#include <Arduino.h>
#include "IoTCore_esp8266.h"
#include "ArduinoJson.h"

void messageReceivedAdvanced(MQTTClient *client, char topic[], char bytes[], int length);
void updateWifiCredHandler(StaticJsonDocument<120> doc);
void otaUpdateHandler(StaticJsonDocument<120> doc);
void resetEnergyHandler(StaticJsonDocument<120> doc);
void connectToHandler(StaticJsonDocument<120> doc);

// Forward declarations
String getDeviceState();