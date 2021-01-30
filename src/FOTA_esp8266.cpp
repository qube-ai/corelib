#ifdef ESP8266

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include "device.h"

#ifndef APSSID
#define APSSID "Curiosity"
#define APPSK "duckduckgo123"
#endif

ESP8266WiFiMulti WiFiMulti;

String host = "storage.googleapis.com";
WiFiClient client;

// Variables to validate response from GCP
long contentLength = 0;
bool isValidContentType = false;

void setupForOTA()
{
    Serial.println();
    Serial.println();
    Serial.println();

    for (uint8_t t = 4; t > 0; t--)
    {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(APSSID, APPSK);
}

void update_started()
{
    Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished()
{
    Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total)
{
    Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err)
{
    Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}

void performOTAUpdate(String version)
{
    Serial.println("Setting up for an OTA update");
    setupForOTA();

    // wait for WiFi connection
    if ((WiFiMulti.run() == WL_CONNECTED))
    {

        WiFiClient client;

        // The line below is optional. It can be used to blink the LED on the board during flashing
        // The LED will be on during download of one buffer of data from the network. The LED will
        // be off during writing that buffer to flash
        // On a good connection the LED should flash regularly. On a bad connection the LED will be
        // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
        // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
        ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
        ESPhttpUpdate.rebootOnUpdate(true);

        // Add optional callback notifiers
        ESPhttpUpdate.onStart(update_started);
        ESPhttpUpdate.onEnd(update_finished);
        ESPhttpUpdate.onProgress(update_progress);
        ESPhttpUpdate.onError(update_error);

        Serial.println("Starting OTA operation.");

        // Check if the version number is correct.
        if (version.length() == 0)
        {
            Serial.println("Please specify a version number.");
            return;
        }

        // Prepare the firmware and checksum download URL
        String firmwareRelativeURL = "/ota-server/" + version + "/firmware.bin";
        // String checksumRelativeURL = "/ota-server/" + version + "/checksum.txt";
        Serial.println("Firmware relative download URL: " + firmwareRelativeURL);
        // Serial.println("Checksum URL: " + checksumRelativeURL);
        // String finalURL = "http://" + host + "/" + firmwareRelativeURL;

        // http://storage.googleapis.com/ota-server/1.0.0-test/firmware.bin

        t_httpUpdate_return ret = ESPhttpUpdate.update(client, host, 80, firmwareRelativeURL);
        // t_httpUpdate_return ret = ESPhttpUpdate.update(client, finalURL);
        // t_httpUpdate_return ret = ESPhttpUpdate.update(host, 80, firmwareRelativeURL);

        switch (ret)
        {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            break;

        case HTTP_UPDATE_OK:
            Serial.println("HTTP_UPDATE_OK");
            break;
        
        default:
            Serial.printf("Unknow value of ret: %d", ret);
        }
    }

    else {
        Serial.println("Not connected to the network!");
    }
}

#endif