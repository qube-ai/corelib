#pragma once

// Basic Arduino stuff
#include <Arduino.h>

// WiFi Networking
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>

// Library imports
#include "ArduinoJson.h"
#include "CloudIoTCore.h"
#include "CloudIoTCoreDevice.h"
#include "CloudIoTCoreMqtt.h"
#include "LittleFS.h"
#include "MQTT.h"

// Corelib imports
#if defined(CORELIB_STATUS_LED)
    #include <jled.h>
#endif

// Corelib imports
#if defined(CORELIB_STORAGE)
    #include "common/Storage.h"
#endif
#if defined(CORELIB_WIFI_MANAGER)
    #include "common/WiFiManager.h"
#endif

#if defined(CORELIB_STATUS_LED)
    #include "common/statusLed.h"
#endif

// To publish data to Cloud IoT Core
namespace iotcore {
    // To publish data to Cloud IoT Core
    bool publishTelemetry(String data);
    bool publishTelemetry(const char *data, int length);
    bool publishTelemetry(String subfolder, String data);
    bool publishTelemetry(String subfolder, const char *data, int length);
    bool publishState(String data);

    /**
     * @brief Setup connection to Cloud IoT Core
     *
     */
    void setupCloudIoT();

    /**
     * @brief Maintains connection to Cloud IoT Core
     *
     */
    void mainLoop();

    /**
     * @brief Check's if we are connected to MQTT server (this method is still
     * in use by status led code)
     *
     * @return true Connected to MQTT server
     * @return false Not connected to MQTT server
     */
    bool connectedToMqtt();
}  // namespace iotcore

extern char received_data[50];

#ifdef UNIT_TEST
extern String lastReceivedMessage;
extern bool messageAvailable;
#endif