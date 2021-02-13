#pragma once

#include <Arduino.h>

#include "ArduinoJson.h"
#include "IoTCore_esp8266.h"

// Can't put this into a namespace (this is shared between cloud iot core
// library)
void messageReceivedAdvanced(MQTTClient *client, char topic[], char bytes[],
                             int length);

namespace messageHandler {
    void updateWifiCredHandler(StaticJsonDocument<120> doc);
    void otaUpdateHandler(StaticJsonDocument<120> doc);
    void resetEnergyHandler(StaticJsonDocument<120> doc);
    void connectToHandler(StaticJsonDocument<120> doc);
}  // namespace messageHandler

// Forward declarations - Don't use this
String getDeviceState();