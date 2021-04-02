#include "corelib.h"

// TODO make the return type for this to be bool,
// This would allow us to check whether we were able
// to setup the device correctly or not.
void corelib::setup() {
    // If Serial has not been initialized
    // Setup serial connection
    if (!Serial) { Serial.begin(115200); }

    delay(1000);

#if defined(CORELIB_STATUS_LED)
    status_led::setup_started();
#endif

    Serial.println("Starting setup ");

#if defined(CORELIB_STORAGE)
    storage::init();
    Serial.println("initStorage() executed.");
#endif

#if defined(CORELIB_IOTCORE)
    iotcore::setupCloudIoT();
    Serial.println("setupCloudIoT() executed.");
#endif

#if defined(CORELIB_APP_COMM)
    appcomm::setupAppCommunication();
    Serial.println("setupAppCommunication() executed.");
#endif

    Serial.println("Setup complete");

#if defined(CORELIB_STATUS_LED)
    status_led::setup_complete();
#endif
}

void corelib::loop() {
#if defined(CORELIB_IOTCORE)
    iotcore::mainLoop();
#endif
}
