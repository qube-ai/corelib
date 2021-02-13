#include <Arduino.h>
#include "Storage.h"
#include "WiFiManager.h"
#include "statusLed.h"
#include "appComm.h"

#ifdef ESP8266
#include "IoTCore_esp8266.h"
#include "FOTA_esp8266.h"
#endif

#ifdef ESP32
#include "IoTCore_esp32.h"
#include "FOTA_esp32.h"
#endif

/*
* Default baud rate at which the device should 
* communicate.
*/
#define BAUD_RATE 9600

namespace corelib {
    void setup();
    void loop();
}