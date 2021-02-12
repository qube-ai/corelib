#include "Storage.h"
#include "WiFiManager.h"

#ifdef ESP8266
#include "IoTCore_esp8266.h"
#include "FOTA_esp8266.h"
#endif

#ifdef ESP32
#include "IoTCore_esp32.h"
#include "FOTA_esp32.h"
#endif