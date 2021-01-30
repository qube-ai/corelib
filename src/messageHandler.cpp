#include <Arduino.h>
#include "ArduinoJson.h"
#include "FOTA.h"
#include "device.h"
#include "IoTCore.h"
#include "Storage.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

void updateWifiCredHandler(StaticJsonDocument<120> doc)
{
    Serial.println("Received a command to change WiFi cred");
    short index = doc["index"];
    if (index >= 0 && index <= 4)
    {
        wifi_cred temp = getWiFiCreds(index);

        Serial.print("Replacing SSID(");
        Serial.print(temp.ssid);
        Serial.println(")");

        const char *ssid = doc["ssid"];
        std::string ssid_string(ssid);
        const char *pass = doc["pass"];
        std::string pass_string(pass);

        setWiFiCreds(index, ssid_string, pass_string);
        Serial.println("New WiFi credentials were saved successfully.");
    }
    else
    {
        Serial.print("Unable to update SSID and PASS. Index value out of range: ");
        Serial.println(index);
    }
}

void otaUpdateHandler(StaticJsonDocument<120> doc)
{
    Serial.println("Received a command to perform OTA update");
    String version = doc["ota_update"];
    performOTAUpdate(version);
}

void resetEnergyHandler(StaticJsonDocument<120> doc)
{
    resetEnergyReading();
}

void connectToHandler(StaticJsonDocument<120> doc)
{
    wifi_cred creds = getWiFiCreds(doc["connect_to"]);

    if (strlen(creds.ssid) > 0 && strlen(creds.password) > 0)
    {
        Serial.println("Disconnecting from exisiting network.");
        WiFi.disconnect();

        Serial.println("Connecting to a different SSID index");
        WiFi.begin(creds.ssid, creds.password);

        Serial.print("Connecting");
        long start_time = millis();
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
            if ((millis() - start_time) > 30000)
            {
                Serial.println("Unable to connect to the specified network.");
                break;
            }
        }
    }
    else
    {
        Serial.println("No SSID and Pass found at specified index.");
    }
}

void messageReceivedAdvanced(MQTTClient *client, char topic[], char bytes[], int length)
{
    Serial.print("Incoming: ");
    Serial.print(topic);
    Serial.print(" - ");
    Serial.println(bytes);

    StaticJsonDocument<120> doc;
    deserializeJson(doc, bytes);
    bool sendNewStateMessage = false;

    if (doc.containsKey("ssid") && doc.containsKey("pass") && doc.containsKey("index"))
    {
        updateWifiCredHandler(doc);
        sendNewStateMessage = true;
    }

    else if (doc.containsKey("ota_update"))
    {
        otaUpdateHandler(doc);
        sendNewStateMessage = true;
    }

    else if (doc.containsKey("reset_energy"))
    {
        resetEnergyHandler(doc);
    }

    else if (doc.containsKey("connect_to"))
    {
        connectToHandler(doc);
    }

    else
    {
        Serial.println("A message was received that couldn't be parsed.");
    }

    // Check if state needs to be resent
    if (sendNewStateMessage)
    {
        // Send a state update back to the server
        String stateInfo = getDeviceState();
        Serial.print("Sending state update as relay state has changed -> ");
        Serial.println(stateInfo);
        publishState(stateInfo);
    }
}