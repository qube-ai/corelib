#pragma once

#include <Arduino.h>

#include "ArduinoJson.h"
#include "esp32/IoTCore_esp32.h"
#include "common/Storage.h"

#if defined(ESP8266)
#include "IoTCore_esp8266.h"
#endif

// Can't put this into a namespace (this is shared between cloud iot core
// library)
void messageReceivedAdvanced(MQTTClient *client, char topic[], char bytes[],
                             int length);

namespace messageHandler {
    void updateWifiCredHandler(StaticJsonDocument<120> doc);
    void otaUpdateHandler(StaticJsonDocument<120> doc);
    void registerUserCallback(void (*messageReceivedUserCallback)(MQTTClient *client, char topic[], char bytes[], int length));
}  // namespace messageHandler

// Forward declarations - Don't use this
String getDeviceState();