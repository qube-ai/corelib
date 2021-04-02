#if defined(RELEASE)

    #include <Arduino.h>

    #include "corelib.h"

void myCallback(MQTTClient *client, char topic[], char bytes[], int length) {
    Serial.println("myCallback was called.");
}

String deviceState() {
    String state = "{\"firmware_version\": \"1.0.0\"}";
    return state;
}

void setup() {
    Serial.begin(115200);
    delay(100);

    corelib::setup();
    Serial.println("corelib setup complete.");

    // Setup callback for IoTCore
    messageHandler::registerUserCallback(myCallback);
    messageHandler::registerDeviceStateFunction(deviceState);

    Serial.println("All work in setup section is complete.");
}

void loop() { corelib::loop(); }

#endif