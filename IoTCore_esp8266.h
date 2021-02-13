#pragma once

#include <Arduino.h>
#include <jled.h>

#include "CloudIoTCoreMqtt.h"
#include "Storage.h"
#include "WiFiManager.h"
// #include "messageHandler.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>

#include "ArduinoJson.h"
#include "CloudIoTCore.h"
#include "CloudIoTCoreDevice.h"
#include "CloudIoTCoreMqtt.h"
#include "LittleFS.h"
#include "MQTT.h"
#include "statusLed.h"

// To publish data to Cloud IoT Core
namespace iotcore {
    bool publishTelemetry(String data);
    bool publishTelemetry(const char *data, int length);
    bool publishTelemetry(String subfolder, String data);
    bool publishTelemetry(String subfolder, const char *data, int length);
    bool publishState(String data);
    void setupCloudIoT();
    void mqttLoop();
    bool connectedToMqtt();
    void logSomeShit();
}  // namespace iotcore

extern char received_data[50];

#ifdef UNIT_TEST
extern String lastReceivedMessage;
extern bool messageAvailable;
#endif