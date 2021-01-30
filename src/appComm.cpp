#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "ArduinoJson.h"
#include "messageHandler.h"

WiFiUDP Udp;
unsigned int localUdpPort = 4000;                     // local port to listen on
char incomingPacket[255];                             // buffer for incoming packets
char replyPacket[] = "Hi there! Got the message :-)"; // a reply string to send back

IPAddress mobile_ip;
uint16_t mobile_port;
bool send_to_mobile = false;

// Forward declaration of device state
String getDeviceState();

void setupAppCommunication()
{
    Udp.begin(localUdpPort);
    Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void checkForAppCommMessages()
{
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
        // receive incoming UDP packets
        Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
        int len = Udp.read(incomingPacket, 255);
        if (len > 0)
        {
            incomingPacket[len] = 0;
        }

        // char temp[20];
        // strncpy(temp, incomingPacket, 9);

        // Serial.print("Value of temp -> ");
        // Serial.println(temp);
        char firstChar[2] = "";
        memcpy(firstChar, incomingPacket, 1);
        // strncpy(firstChar, incomingPacket, 2);
        Serial.printf("Value of first Char is -> %s\n", firstChar);

        if (strcmp(incomingPacket, "broadcast\n") == 0)
        {

            Serial.println("Received a broadcast message on UDP port. Sending back device IP address.");
            mobile_ip = Udp.remoteIP();
            mobile_port = Udp.remotePort();
            send_to_mobile = true;

            Udp.beginPacket(mobile_ip, mobile_port);

            StaticJsonDocument<120> doc;
            doc["ip_address"] = WiFi.localIP().toString();
            String final_data;
            serializeJson(doc, final_data);

            char temp_array[100] = "";
            final_data.toCharArray(temp_array, 100);

            Udp.write(temp_array);
            Udp.endPacket();
        }

        else if (strcmp(incomingPacket, "values\n") == 0)
        {
            Serial.println("Sending device values to phone.");
            String data = getDeviceState();
            char data_array[700];
            data.toCharArray(data_array, data.length() + 3);

            // Actually sending the packet
            Udp.beginPacket(mobile_ip, mobile_port);
            Udp.write(data_array);
            Udp.endPacket();
        }

        else if (strcmp(firstChar, "{") == 0)
        {
            Serial.println("Parsing incoming JSON...");
            StaticJsonDocument<120> doc;
            deserializeJson(doc, incomingPacket);

            if (doc.containsKey("ssid") && doc.containsKey("pass") && doc.containsKey("index"))
            {
                updateWifiCredHandler(doc);
            }

            else if (doc.containsKey("ota_update"))
            {
                otaUpdateHandler(doc);
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
        }

        else
        {
            Serial.println("Got a message on UDP that the device couldn't understand.");
        }

        Serial.printf("UDP packet contents: %s\n", incomingPacket);

        // send back a reply, to the IP address and port we got the packet from
        // Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        // Udp.write(replyPacket);
        // Udp.endPacket();
    }
}

void sendAppCommMessage(String data)
{
    if (send_to_mobile)
    {
        Serial.println("Sending data to mobile device");
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        // Udp.write(data.c_str());
        Udp.write("device data");
        Udp.endPacket();
    }
    else
    {
        Serial.println("Don't have mobile IP address to send values to.");
    }
}
