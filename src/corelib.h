#include <Arduino.h>

#if defined(CORELIB_STORAGE)
    #include "common/Storage.h"
#endif

#if defined(CORELIB_WIFI_MANAGER)
    #include "common/WiFiManager.h"
#endif

#if defined(CORELIB_APP_COMM)
    #include "common/appComm.h"
#endif

#if defined(CORELIB_STATUS_LED)
    #include "common/statusLed.h"
#endif

#if defined(ESP32) && defined(CORELIB_FOTA)
    #include "esp32/FOTA_esp32.h"
#endif

#if defined(ESP32) && defined(CORELIB_IOTCORE)
    #include "esp32/IoTCore_esp32.h"
#endif

#if defined(ESP8266) && defined(CORELIB_FOTA)
    #include "esp8266/FOTA_esp8266.h"
#endif

#if defined(ESP8266) && defined(CORELIB_IOTCORE)
    #include "esp8266/IoTCore_esp8266.h"
#endif

namespace corelib {
    void setup();
    void loop();
}  // namespace corelib