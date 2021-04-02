#include <Arduino.h>

#include "ArduinoJson.h"
#include "corelib.h"

void myCallback(MQTTClient *client, char topic[], StaticJsonDocument<120> doc) {
    // Serialize and create a new JSON string representation
    String final_payload;
    serializeJson(doc, final_payload);
    Serial.print("myCallback payload -> ");
    Serial.println(final_payload);
}

String deviceState() {
    String state = "{\"firmware_version\": \"1.0.0\"}";
    return state;
}

void setup() {
    Serial.begin(115200);
    delay(100);

    corelib::setup();
    Serial.println("corelib setup complete.");

    // Set WiFi creds
    storage::setWiFiCreds(0, "Apoorva", "testing123");
    storage::setWiFiCreds(1, "Hrithik", "testing123");
    storage::setWiFiCreds(2, "", "");
    storage::setWiFiCreds(3, "", "");
    storage::setWiFiCreds(4, "", "");

    // Get WiFi Creds
    storage::wifi_cred cred1 = storage::getWiFiCreds(0);
    storage::wifi_cred cred2 = storage::getWiFiCreds(1);

    Serial.printf("Credential 1 SSID: %s, Pass: %s\n", cred1.ssid,
                  cred1.password);
    Serial.printf("Credential 2 SSID: %s, Pass: %s\n", cred2.ssid,
                  cred2.password);

    // Set IoTCore details
    storage::setProjectID("podnet-switch");
    storage::setLocation("us-central1");
    storage::setRegistryID("test-registry");
    storage::setDeviceID("test-device-2");
    storage::setTimezone(19800);

    // Get IoTCore details
    char project_id[20] = "";
    storage::getProjectID(project_id);
    Serial.printf("Project ID -> %s\n", project_id);

    char location[20] = "";
    storage::getLocation(location);
    Serial.printf("Location -> %s\n", location);

    char registry_id[20] = "";
    storage::getRegistryID(registry_id);
    Serial.printf("Registry ID -> %s\n", registry_id);

    char device_id[20] = "";
    storage::getDeviceID(device_id);
    Serial.printf("Device ID -> %s\n", device_id);

    // Setup callback for IoTCore
    messageHandler::registerUserCallback(myCallback);
    messageHandler::registerDeviceStateFunction(deviceState);

    Serial.println("All work in setup section is complete.");
}

void loop() { corelib::loop(); }