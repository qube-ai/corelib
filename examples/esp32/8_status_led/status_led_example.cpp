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
    status_led::setup_started();
    Serial.println("corelib setup complete.");

    // Setup callback for IoTCore
    messageHandler::registerUserCallback(myCallback);
    messageHandler::registerDeviceStateFunction(deviceState);

    Serial.println("All work in setup section is complete.");
    status_led::setup_complete();
}

void loop() { corelib::loop(); }
