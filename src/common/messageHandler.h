#pragma once

// Base imports
#include <Arduino.h>

// Library imports
#include "ArduinoJson.h"
#include "CloudIoTCore.h"
#include "CloudIoTCoreDevice.h"
#include "CloudIoTCoreMqtt.h"
#include "MQTT.h"

// Corelib imports
#include "common/Storage.h"

#if defined(ESP32)
    #include "esp32/IoTCore_esp32.h"
#endif

#if defined(ESP8266)
    #include "esp8266/IoTCore_esp8266.h"
#endif

// Can't put this into a namespace (this is shared between cloud iot core
// library)
void messageReceivedAdvanced(MQTTClient *client, char topic[], char bytes[],
                             int length);

namespace messageHandler {
    void updateWifiCredHandler(StaticJsonDocument<120> doc);
    void otaUpdateHandler(StaticJsonDocument<120> doc);
    void registerUserCallback(void (*messageReceivedUserCallback)(
        MQTTClient *client, char topic[], StaticJsonDocument<120> doc));
    void registerDeviceStateFunction(String (*deviceStateCallback)());
}  // namespace messageHandler

// Forward declarations - Don't use this
String getDeviceState();