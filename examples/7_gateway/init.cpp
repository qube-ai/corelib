#if defined(INIT_EEPROM)

    #include <Arduino.h>

    #include "corelib.h"

void print_creds(storage::wifi_cred t) {
    Serial.print("SSID = ");
    Serial.print(t.ssid);
    Serial.print("Pass = ");
    Serial.println(t.password);
}

void setup() {
    Serial.begin(115200);
    while (!Serial)
        ;

    corelib::setup();

    // Set WiFi creds
    storage::setWiFiCreds(0, "Qube", "Deepwaves007");
    storage::setWiFiCreds(1, "Hrithik", "testing123");
    storage::setWiFiCreds(2, "", "");
    storage::setWiFiCreds(3, "", "");
    storage::setWiFiCreds(4, "", "");

    // Get WiFi Creds
    storage::wifi_cred cred1 = storage::getWiFiCreds(0);
    storage::wifi_cred cred2 = storage::getWiFiCreds(1);
    storage::wifi_cred cred3 = storage::getWiFiCreds(2);
    storage::wifi_cred cred4 = storage::getWiFiCreds(3);
    storage::wifi_cred cred5 = storage::getWiFiCreds(4);

    Serial.printf("Credential 1 SSID: %s, Pass: %s\n", cred1.ssid,
                  cred1.password);
    Serial.printf("Credential 2 SSID: %s, Pass: %s\n", cred2.ssid,
                  cred2.password);
    Serial.printf("Credential 3 SSID: %s, Pass: %s\n", cred3.ssid,
                  cred3.password);
    Serial.printf("Credential 4 SSID: %s, Pass: %s\n", cred4.ssid,
                  cred4.password);
    Serial.printf("Credential 5 SSID: %s, Pass: %s\n", cred5.ssid,
                  cred5.password);

    // Set IoTCore details
    storage::setProjectID("podnet-switch");
    storage::setLocation("us-central1");
    storage::setRegistryID("gen-2");
    storage::setDeviceID("GT2-A-2");
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

    Serial.println("Device Setup complete.");
}

void loop() {}

#endif