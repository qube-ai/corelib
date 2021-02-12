#include "WiFiManager.h"
#include "Storage.h"
#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#elif ESP32
#include <WiFi.h>
#endif

#include <WiFiClientSecure.h>

// Configuration for NTP
const char *ntp_primary = "pool.ntp.org";
const char *ntp_secondary = "time.nist.gov";

short CURRENT_STATE = NOT_CONNECTED;
short CURRENT_SSID_INDEX = 0;

ESP8266WiFiMulti wifiMulti;
boolean connectioWasAlive = true;
bool apsAdded = false;

void setupWiFi()
{
    if (!apsAdded)
    {
        // Connecting to a WiFi Network
        for (short i = 0; i < 5; i++)
        {
            wifi_cred creds = getWiFiCreds(i);
            if (strlen(creds.ssid) > 0 && strlen(creds.password) > 0)
            {
                wifiMulti.addAP(creds.ssid, creds.password);
            }
            else
            {
                Serial.printf("Not adding WiFi creds at index %d\n", i);
            }
        }
        apsAdded = true;
    }

    Serial.println("Connecting to WiFi network...");
    while (wifiMulti.run() != WL_CONNECTED)
    {
        // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
        delay(1000);
        Serial.print('.');
    }
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID()); // Tell us what network we're connected to
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());

    long current_timezone = 0;
    getTimezone(&current_timezone);
    Serial.printf("Current timezone -> %lds\n", current_timezone);
    configTime(current_timezone, 0, ntp_primary, ntp_secondary);

    // Check if time is less than, 10th Oct, 2020. Keep delaying till we get the correct time.
    unsigned long temp = millis();
    bool timeSyncComplete = false;
    while ((millis() - temp) < 30 * 1000)
    {
        if (time(nullptr) > 1602374400)
        {
            Serial.println("TIME SYNCED");
            timeSyncComplete = true;
            break;
        }
        else
        {
            delay(1000);
            Serial.print(".");
        }
    }
}

void reconnectWiFi(bool log=true)
{
    if (log)
    {
        Serial.println("Connecting to WiFi network...");
    }
    wifiMulti.run();

    if (log)
    {
        Serial.println('\n');
        Serial.print("Connected to ");
        Serial.println(WiFi.SSID()); // Tell us what network we're connected to
        Serial.print("IP address:\t");
        Serial.println(WiFi.localIP());
    }
}
