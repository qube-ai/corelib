#pragma once

// Basic Arduino stuff
#include <Arduino.h>
#include <WiFiClientSecure.h>

// Higher level libraries in use
#include "ArduinoJson.h"
#include "CloudIoTCore.h"
#include "CloudIoTCoreDevice.h"
#include "CloudIoTCoreMqtt.h"
#include "MQTT.h"
#include "SPIFFS.h"

// Corelib imports
#include "../common/Storage.h"
#include "../common/WiFiManager.h"
#include "../common/messageHandler.h"

namespace iotcore {
    // To publish data to Cloud IoT Core
    bool publishTelemetry(String data);
    bool publishTelemetry(const char* data, int length);
    bool publishTelemetry(String subfolder, String data);
    bool publishTelemetry(String subfolder, const char* data, int length);
    bool publishState(String data);

#if defined(CORELIB_GATEWAY)
    bool publishDelegateTelemetry(String delegateId, String data);
    bool publishDelegateState(String delegateId, String data);
#endif

    void mainLoop();

    void setupCloudIoT();

    void mqttLoop();

    bool connectedToMqtt();
}  // namespace iotcore

extern char received_data[50];

#ifdef UNIT_TEST
extern String lastReceivedMessage;
extern bool messageAvailable;
#endif