#include "WiFiManager.h"

// Configuration for NTP
const char *ntp_primary = "pool.ntp.org";
const char *ntp_secondary = "time.nist.gov";

short CURRENT_STATE = NOT_CONNECTED;
short CURRENT_SSID_INDEX = 0;

ESP8266WiFiMulti wifiMulti;
boolean connectioWasAlive = true;
bool apsAdded = false;

void wifiman::setupWiFi() {
    if (!apsAdded) {
        // Connecting to a WiFi Network
        for (short i = 0; i < 5; i++) {
            storage::wifi_cred creds = storage::getWiFiCreds(i);
            if (strlen(creds.ssid) > 0 && strlen(creds.password) > 0) {
                wifiMulti.addAP(creds.ssid, creds.password);
            } else {
                Serial.printf("Not adding WiFi creds at index %d\n", i);
            }
        }
        apsAdded = true;
    }

    Serial.println("Connecting to WiFi network...");
    while (wifiMulti.run() != WL_CONNECTED) {
        // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect
        // to the strongest of the networks above
        delay(1000);
        Serial.print('.');
    }
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());  // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());

    long current_timezone = 0;
    storage::getTimezone(&current_timezone);
    Serial.printf("Current timezone -> %lds\n", current_timezone);
    configTime(current_timezone, 0, ntp_primary, ntp_secondary);

    // Check if time is less than, 10th Oct, 2020. Keep delaying till we get the
    // correct time.
    unsigned long temp = millis();

    while ((millis() - temp) < 30 * 1000) {
        if (time(nullptr) > 1602374400) {
            Serial.println("TIME SYNCED");
            break;
        } else {
            delay(1000);
            Serial.print(".");
        }
    }
}

void wifiman::reconnectWiFi(bool log = true) {
    if (log) { Serial.println("Connecting to WiFi network..."); }
    wifiMulti.run();

    if (log) {
        Serial.println('\n');
        Serial.print("Connected to ");
        Serial.println(WiFi.SSID());  // Tell us what network we're connected to
        Serial.print("IP address:\t");
        Serial.println(WiFi.localIP());
    }
}
