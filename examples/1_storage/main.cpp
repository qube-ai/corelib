#include <Arduino.h>
#include "corelib.h"

void setup() {
    Serial.begin(115200);
    delay(100);

    corelib::setup();
    Serial.println("corelib setup complete.");

    storage::setDeviceID("test-device-id");

    char device_id[20] = "";
    storage::getDeviceID(device_id);
    Serial.printf("Device ID is %s\n", device_id);

    Serial.println("All work in setup section is complete.");

}

void loop() {
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);

    
    corelib::loop();
}