#ifdef ESP8266

#include "IoTCore.h"

#include "WiFiManager.h"
#include "Storage.h"
#include "messageHandler.h"

#include "MQTT.h"
#include "CloudIoTCore.h"
#include "CloudIoTCoreMqtt.h"
#include "CloudIoTCoreDevice.h"
#include "ArduinoJson.h"

#include <Arduino.h>
#include <WiFiClientSecureBearSSL.h>
#include "LittleFS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include "statusLed.h"

char project_id[33] = "";
char location[32] = "";
char registry_id[32] = "";
char device_id[32] = "";

char received_data[50] = "";

static CloudIoTCoreMqtt *mqtt;
static MQTTClient *mqttClient;
static BearSSL::WiFiClientSecure netClient;
static BearSSL::X509List certList;
static CloudIoTCoreDevice device;

unsigned long iat = 0;

// Time (seconds) to expire token += 20 minutes for drift
const int jwt_exp_secs = 3600 * 24; // Maximum 24H (3600*24)

// In case we ever need extra topics
const int ex_num_topics = 0;
const char *ex_topics[ex_num_topics];

static void readDerCert(const char *filename)
{
    File ca = LittleFS.open(filename, "r");
    if (ca)
    {
        size_t size = ca.size();
        uint8_t cert[size];
        ca.read(cert, size);
        certList.append(cert, size);
        ca.close();

#ifndef UNIT_TEST
        Serial.print("Success to open ca file ");
#endif
    }
    else
    {
#ifndef UNIT_TEST
        Serial.print("Failed to open ca file ");
#endif
    }

#ifndef UNIT_TEST
    Serial.println(filename);
#endif
}

static void setupCertAndPrivateKey()
{
    // If using the (preferred) method with the cert and private key in /data (SPIFFS)
    // To get the private key run
    // openssl ec -in <private-key.pem> -outform DER -out private-key.der

    if (!LittleFS.begin())
    {
#ifndef UNIT_TEST
        Serial.println("Failed to mount file system");
#endif

        return;
    }

    readDerCert("/primary_ca.crt"); // primary_ca.pem
    readDerCert("/backup_ca.crt");  // backup_ca.pem
    netClient.setTrustAnchors(&certList);

    File f = LittleFS.open("/private-key.der", "r");
    if (f)
    {
        size_t size = f.size();
        uint8_t data[size];
        f.read(data, size);
        f.close();

        BearSSL::PrivateKey pk(data, size);
        device.setPrivateKey(pk.getEC()->x);

#ifndef UNIT_TEST
        Serial.println("Success to open private-key.der");
#endif
    }
    else
    {
#ifndef UNIT_TEST
        Serial.println("Failed to open private-key.der");
#endif
    }

    LittleFS.end();
}

bool publishTelemetry(String data)
{
    return mqtt->publishTelemetry(data);
}

bool publishTelemetry(const char *data, int length)
{
    return mqtt->publishTelemetry(data, length);
}

bool publishTelemetry(String subfolder, String data)
{
    return mqtt->publishTelemetry(subfolder, data);
}

bool publishTelemetry(String subfolder, const char *data, int length)
{
    return mqtt->publishTelemetry(subfolder, data, length);
}

bool publishState(String data)
{
    return mqtt->publishState(data);
}

// void messageReceivedAdvanced(MQTTClient *client, char topic[], char bytes[], int length)
// {
//     if (length > 0)
//     {
//         Serial.printf("incoming: %s - %s\n", topic, bytes);
//     }
//     else
//     {
//         Serial.printf("0\n"); // Success but no message
//     }
// }

String getJwt()
{
    // Disable software watchdog as these operations can take a while.
    ESP.wdtDisable();
    time_t iat = time(nullptr);

#ifndef UNIT_TEST
    Serial.println("Refreshing JWT");
#endif

    String jwt = device.createJWT(iat, jwt_exp_secs);
    ESP.wdtEnable(0);
    return jwt;
}

void connect()
{
    Serial.println("[iotcore_esp8266] inside connect()");
    reconnectWiFi(true);
    mqtt->mqttConnectAsync();
}

void setupCloudIoT()
{
    // Get all the data from storage
    getProjectID(project_id);
    getLocation(location);
    getRegistryID(registry_id);
    getDeviceID(device_id);

    // Setup device object
    device.setProjectId(project_id);
    device.setLocation(location);
    device.setRegistryId(registry_id);
    device.setDeviceId(device_id);

    // ESP8266 WiFi setup
    setupWiFi();

    // ESP8266 WiFi secure initialization and device private key
    setupCertAndPrivateKey();

    mqttClient = new MQTTClient(512);

    mqttClient->setOptions(1000, true, 1000); // keepAlive, cleanSession, timeout
    mqtt = new CloudIoTCoreMqtt(mqttClient, &netClient, &device);
    mqtt->setLogConnect(false);
    mqtt->setUseLts(true);
    mqtt->startMQTTAdvanced(); // Opens connection using advanced callback
}

void mqttLoop()
{
    status_led_update();
    bool connected = mqtt->loop();

    if (!connected) {
        mqtt->mqttConnectAsync();
    }
    status_led_update();
}

bool connectedToMqtt()
{
    return mqtt->loop();
}

void logSomeShit()
{
    // mqtt->logError();
    mqtt->logReturnCode();
    // mqtt->logConfiguration(false);
}

#endif