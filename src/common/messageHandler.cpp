#if defined(CORELIB_IOTCORE)

    #include "messageHandler.h"

    #if defined(ESP32)
        #include "esp32/FOTA_esp32.h"
    #endif

    #if defined(ESP8266)
        #include "esp8266/FOTA_esp8266.h"
    #endif

// // Forward declaring user defined userMessageReceived() function
// void userMessageReceived(MQTTClient *client, char topic[], char bytes[],
//                          int length);

bool userMessageCallbackRegistered = false;
void (*messageReceivedUserCallback)(MQTTClient *client, char topic[],
                                    StaticJsonDocument<120> doc);

bool deviceStateCallbackRegistered = false;
String (*deviceStateCallback)();

    #if defined(CORELIB_FOTA)
void messageHandler::otaUpdateHandler(StaticJsonDocument<120> doc) {
    Serial.println("Received a message to perform an OTA update.");
    if (doc.containsKey("version")) {
        fota::performOTAUpdate(doc["version"]);
    } else {
        Serial.println("version key not found in incoming JSON");
    }
}
    #endif

    #if defined(CORELIB_GATEWAY)
void addNodeToGatewayHandler(StaticJsonDocument<120> doc) {
    Serial.println("Received a message to add a node to Gateway.");
    if (doc.containsKey("device_id")) {
        iotcore::addNodeToGateway(doc["device_id"]);
    } else {
        Serial.println("device_id key not found in incoming JSON");
    }
}
    #endif

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

    #if defined(CORELIB_STATUS_LED)
    status_led::downlink_started();
    #endif

    StaticJsonDocument<120> doc;
    deserializeJson(doc, bytes);
    bool sendNewStateMessage = false;

    int msg_type = doc["t"];

    // Update SSID and Pass of device
    if (msg_type == -1) {
        if (doc.containsKey("ssid") && doc.containsKey("pass") &&
            doc.containsKey("index")) {
            messageHandler::updateWifiCredHandler(doc);
            sendNewStateMessage = true;
        } else {
            Serial.println("WiFi cred update msg has improper syntax.");
        }
    }
    #if defined(CORELIB_FOTA)
    else if (msg_type == -2) {
        if (doc.containsKey("version")) {
            messageHandler::otaUpdateHandler(doc);
            sendNewStateMessage = true;
        } else {
            Serial.println("OTA update message has improper syntax.");
        }
    }
    #endif

    #if defined(CORELIB_GATEWAY)
    else if (msg_type == -3) {
        Serial.println("Type -3 message handler was triggered.");
        addNodeToGatewayHandler(doc);
    }
    #endif

    else {
        if (userMessageCallbackRegistered) {
            // char t_topic[100] = "";
            // char t_payload[length] = "";

            // strcpy(t_topic, topic);
            // strcpy(t_payload, bytes);

            messageReceivedUserCallback(client, topic, doc);
        } else {
            Serial.println("user message callback hasn't been registered.");
        }
    }

    // Check if state needs to be resent
    if (sendNewStateMessage) {
        // Send a state update back to the server
        String stateInfo;
        if (deviceStateCallbackRegistered) {
            stateInfo = deviceStateCallback();
        } else {
            stateInfo = "{\"ping\": \"pong\"}";
        }

        Serial.print("Sending state update -> ");
        Serial.println(stateInfo);

        iotcore::publishState(stateInfo);
    }
}

void messageHandler::registerUserCallback(void (*callbackFunction)(
    MQTTClient *client, char topic[], StaticJsonDocument<120> doc)) {
    userMessageCallbackRegistered = true;
    messageReceivedUserCallback = callbackFunction;
    Serial.println("An incoming cloud message handler was assigned.");
}

void messageHandler::registerDeviceStateFunction(String (*callback)()) {
    deviceStateCallbackRegistered = true;
    deviceStateCallback = callback;
    Serial.println("device state generator was registered.");
}

#endif