#include <Arduino.h>

#include "Storage.h"
#include "WiFiManager.h"
#include "appComm.h"
#include "statusLed.h"

#ifdef ESP8266
    #include "FOTA_esp8266.h"
    #include "IoTCore_esp8266.h"
#endif

#ifdef ESP32
    #include "FOTA_esp32.h"
    #include "IoTCore_esp32.h"
#endif

/*
 * Default baud rate at which the device should
 * communicate.
 */
#define BAUD_RATE 9600

namespace corelib {
    void setup();
    void loop();
}  // namespace corelib