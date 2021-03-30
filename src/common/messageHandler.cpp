#if defined(CORELIB_IOTCORE)

#include "messageHandler.h"

// // Forward declaring user defined userMessageReceived() function
// void userMessageReceived(MQTTClient *client, char topic[], char bytes[],
//                          int length);


void messageHandler::otaUpdateHandler(StaticJsonDocument<120> doc) {
    Serial.println("Received a message to perform an OTA update.");
}

void messageHandler::updateWifiCredHandler(StaticJsonDocument<120> doc) {
    Serial.println("Received a command to change WiFi cred");
    short index = doc["index"];
    if (index >= 0 && index <= 4) {
        storage::wifi_cred temp = storage::getWiFiCreds(index);

        Serial.print("Replacing SSID(");
        Serial.print(temp.ssid);
        Serial.println(")");

        const char *ssid = doc["ssid"];
        std::string ssid_string(ssid);
        const char *pass = doc["pass"];
        std::string pass_string(pass);

        storage::setWiFiCreds(index, ssid_string, pass_string);
        Serial.println("New WiFi credentials were saved successfully.");
    } else {
        Serial.print(
            "Unable to update SSID and PASS. Index value out of range: ");
        Serial.println(index);
    }
}

void messageReceivedAdvanced(MQTTClient *client, char topic[], char bytes[],
                             int length) {
    Serial.print("Incoming: ");
    Serial.print(topic);
    Serial.print(" - ");
    Serial.println(bytes);

    StaticJsonDocument<120> doc;
    deserializeJson(doc, bytes);
    bool sendNewStateMessage = false;

    int msg_type = doc["t"];
    
    // Update SSID and Pass of device
    if(msg_type == -1) {
        if (doc.containsKey("ssid") && doc.containsKey("pass") &&
        doc.containsKey("index")) {
        messageHandler::updateWifiCredHandler(doc);
        sendNewStateMessage = true;
        }
        else {
            Serial.println("WiFi cred update msg has improper syntax.");
        }
    }

    else if(msg_type == -2) {
        if (doc.containsKey("version")) {
        messageHandler::otaUpdateHandler(doc);
        sendNewStateMessage = true;
        }
        else {
            Serial.println("OTA update message has improper syntax.");
        }
    }

    else {
        Serial.println("Unknown message type was received.");
        // userMessageReceived(client, topic, bytes, length);
    }

    // Check if state needs to be resent
    if (sendNewStateMessage) {
        // Send a state update back to the server
        // String stateInfo = getDeviceState();
        String stateInfo = "{\"ping\": \"pong\"}";
        
        Serial.print("Sending state update as relay state has changed -> ");
        Serial.println(stateInfo);
        
        iotcore::publishState(stateInfo);
    }
}


#endif