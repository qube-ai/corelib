#include "corelib.h"

// TODO make the return type for this to be bool,
// This would allow us to check whether we were able
// to setup the device correctly or not.
void corelib::setup()
{
    Serial.begin(BAUD_RATE);
    delay(1000);
    status_led::setup_started();

    Serial.println("Starting setup ");

    initStorage();
    Serial.println("initStorage() executed.");

    iotcore::setupCloudIoT();
    Serial.println("setupCloudIoT() executed.");

    appcomm::setupAppCommunication();
    Serial.println("setupAppCommunication() executed.");

    Serial.println("Setup complete");

    status_led::setup_complete();
}


void corelib::loop() {
    iotcore::mqttLoop();
    delay(10);
    wifiman::reconnectWiFi(false);
}
