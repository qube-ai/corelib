#include <Arduino.h>
#include "corelib.h"

void setup() {
    Serial.begin(115200);
    delay(100);

    corelib::setup();
    Serial.println("corelib setup complete.");

    storage::setWiFiCreds(0, "Apoorva", "testing123");
    storage::setWiFiCreds(1, "Hrithik", "testing123");
    storage::setWiFiCreds(2, "", "");
    storage::setWiFiCreds(3, "", "");
    storage::setWiFiCreds(4, "", "");


    storage::wifi_cred cred1  = storage::getWiFiCreds(0);
    storage::wifi_cred cred2  = storage::getWiFiCreds(1);

    Serial.printf("Credential 1 SSID: %s, Pass: %s\n", cred1.ssid, cred1.password);
    Serial.printf("Credential 2 SSID: %s, Pass: %s\n", cred2.ssid, cred2.password);

    wifiman::setupWiFi();

    Serial.println("All work in setup section is complete.");
}

void loop() {
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);

    
    corelib::loop();
}