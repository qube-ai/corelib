#if defined(ESP32) && defined(CORELIB_IOTCORE)

    #include "esp32/IoTCore_esp32.h"

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

    #if defined(CORELIB_GATEWAY)

void attachAndSubscribeNode(String delegateId) {
    // aattach delegate device
    String dat = "{}";
    mqttClient->publish(String("/devices/" + delegateId + "/attach").c_str(),
                        dat.c_str(), false, 1);

    // subscribe to delegate configuration
    mqttClient->subscribe("/devices/" + delegateId + "/config", 1);

    // subscribe to delegate commands
    mqttClient->subscribe("/devices/" + delegateId + "/commands/#", 0);
}

static void setupNodes() {
    if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system.");
        return;
    }

    File f = SPIFFS.open("/devices.txt", "r");
    if (f) {
        // size_t size = f.size();
        // uint8_t data[size];
        // f.read(data, size);

        // 512 bytes can store upto 50 devices
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, f);
        if (error) {
            Serial.println("Failed to read devices.txt file.");
            return;
        }

        JsonArray array = doc.as<JsonArray>();
        for (JsonVariant device : array) {
            Serial.print("Attaching delegate device -> ");
            Serial.println(device.as<String>());
            attachAndSubscribeNode(device.as<String>());
            mqttClient->loop();
        }

        f.close();
    }
}

    #endif

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

bool iotcore::publishTelemetry(String data) {
    return mqtt->publishTelemetry(data);
}

bool iotcore::publishTelemetry(const char *data, int length) {
    return mqtt->publishTelemetry(data, length);
}

bool iotcore::publishTelemetry(String subfolder, String data) {
    return mqtt->publishTelemetry(subfolder, data);
}

bool iotcore::publishTelemetry(String subfolder, const char *data, int length) {
    return mqtt->publishTelemetry(subfolder, data, length);
}

bool iotcore::publishState(String data) { return mqtt->publishState(data); }

    #if defined(CORELIB_GATEWAY)
bool iotcore::publishDelegateTelemetry(String delegateId, String data) {
    return mqttClient->publish(
        String("/devices/" + delegateId + "/events").c_str(),
        String(data).c_str(), false, 1);
}

bool iotcore::publishDelegateState(String delegateId, String data) {
    return mqttClient->publish(
        String("/devices/" + delegateId + "/state").c_str(),
        String(data).c_str(), false, 1);
}
    #endif

String getJwt() {
    iat = time(nullptr);

    #ifndef UNIT_TEST
    Serial.println("Refreshing JWT");
    #endif

    jwt = device->createJWT(iat, jwt_exp_secs);
    return jwt;
}

void connect() {
    wifiman::reconnectWiFi(true);
    mqtt->mqttConnect();
}

void iotcore::setupCloudIoT() {
    // Get all the data from storage
    storage::getProjectID(project_id);
    storage::getLocation(location);
    storage::getRegistryID(registry_id);
    storage::getDeviceID(device_id);

    // TODO get the private key somehow
    char private_key_str[200];
    getPrivateKey(private_key_str);

    device = new CloudIoTCoreDevice(project_id, location, registry_id,
                                    device_id, private_key_str);

    // Setup WiFi
    wifiman::setupWiFi();

    netClient = new WiFiClientSecure();
    mqttClient = new MQTTClient(512);
    mqttClient->setOptions(180, true,
                           10000);  // keepAlive, cleanSession, timeout
    mqtt = new CloudIoTCoreMqtt(mqttClient, netClient, device);
    mqtt->setUseLts(true);
    mqtt->setLogConnect(false);
    mqtt->startMQTTAdvanced();

    #if defined(CORELIB_GATEWAY)
    mqttClient->subscribe("/devices/" + String(device_id) + "/errors", 0);
    #endif
}

void iotcore::mainLoop() {
    mqtt->loop();
    delay(10);

    if (!mqttClient->connected()) {
        wifiman::reconnectWiFi(true);
        mqtt->mqttConnectAsync();

    #if defined(CORELIB_GATEWAY)
        setupNodes();
        delay(500);
    #endif
    }
}

void iotcore::mqttLoop() {
    if (!mqtt->loop()) { mqtt->mqttConnectAsync(); }
}

bool iotcore::connectedToMqtt() { return mqtt->loop(); }

#endif