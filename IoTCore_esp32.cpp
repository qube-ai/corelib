#ifdef ESP32

    #include <Arduino.h>
    #include <WiFiClientSecure.h>

    #include "ArduinoJson.h"
    #include "CloudIoTCore.h"
    #include "CloudIoTCoreDevice.h"
    #include "CloudIoTCoreMqtt.h"
    #include "IoTCore.h"
    #include "MQTT.h"
    #include "SPIFFS.h"
    #include "Storage.h"
    #include "WiFiManager.h"
    #include "messageHandler.h"

char project_id[33] = "";
char location[32] = "";
char registry_id[32] = "";
char device_id[32] = "";

char received_data[50] = "";

Client *netClient;
CloudIoTCoreDevice *device;
CloudIoTCoreMqtt *mqtt;
MQTTClient *mqttClient;
unsigned long iat = 0;
String jwt;

// Time (seconds) to expire token += 20 minutes for drift
const int jwt_exp_secs = 3600 * 24;  // Maximum 24H (3600*24)

// In case we ever need extra topics
const int ex_num_topics = 0;
const char *ex_topics[ex_num_topics];

static void getPrivateKey(char *private_key) {
    // If using the (preferred) method with the cert and private key in /data
    // (SPIFFS) To get the private key run openssl ec -in <private-key.pem>
    // -outform DER -out private-key.der

    // To get the private key run (where private-key.pem is the ec private key
    // used to create the certificate uploaded to google cloud iot):
    // openssl ec -inform DER -in <private-key.pem> -noout -text
    // and copy priv: part.
    // The key length should be exactly the same as the key length bellow (32
    // pairs of hex digits). If it's bigger and it starts with "00:" delete the
    // "00:". If it's smaller add "00:" to the start. If it's too big or too
    // small something is probably wrong with your key.

    if (!SPIFFS.begin()) {
    #ifndef UNIT_TEST
        Serial.println("Failed to mount file system");
    #endif

        return;
    }

    File f = SPIFFS.open("/private-key.txt", "r");
    if (f) {
        size_t size = f.size();
        uint8_t data[size];
        f.read(data, size);
        f.close();

        strcpy(private_key, (char *)data);

    #ifndef UNIT_TEST
        Serial.println("Success to open private-key.txt");
    #endif
    } else {
    #ifndef UNIT_TEST
        Serial.println("Failed to open private-key.txt");
    #endif
    }

    SPIFFS.end();
}

bool publishTelemetry(String data) { return mqtt->publishTelemetry(data); }

bool publishTelemetry(const char *data, int length) {
    return mqtt->publishTelemetry(data, length);
}

bool publishTelemetry(String subfolder, String data) {
    return mqtt->publishTelemetry(subfolder, data);
}

bool publishTelemetry(String subfolder, const char *data, int length) {
    return mqtt->publishTelemetry(subfolder, data, length);
}

bool publishState(String data) { return mqtt->publishState(data); }

String getJwt() {
    iat = time(nullptr);

    #ifndef UNIT_TEST
    Serial.println("Refreshing JWT");
    #endif

    jwt = device->createJWT(iat, jwt_exp_secs);
    return jwt;
}

void connect() {
    reconnectWiFi();
    mqtt->mqttConnect();
}

void setupCloudIoT() {
    // Get all the data from storage
    getProjectID(project_id);
    getLocation(location);
    getRegistryID(registry_id);
    getDeviceID(device_id);

    // TODO get the private key somehow
    char private_key_str[200];
    getPrivateKey(private_key_str);

    device = new CloudIoTCoreDevice(project_id, location, registry_id,
                                    device_id, private_key_str);

    // Setup WiFi
    setupWiFi();

    netClient = new WiFiClientSecure();
    mqttClient = new MQTTClient(512);
    mqttClient->setOptions(180, true,
                           1000);  // keepAlive, cleanSession, timeout
    mqtt = new CloudIoTCoreMqtt(mqttClient, netClient, device);
    mqtt->setUseLts(true);
    mqtt->setLogConnect(false);
    mqtt->startMQTTAdvanced();
}

void mqttLoop() {
    if (!mqtt->loop()) { mqtt->mqttConnect(); }
}

bool connectedToMqtt() { return mqtt->loop(); }

#endif