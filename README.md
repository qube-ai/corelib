# corelib
Library for faster development of IoT devices.

## Defaults
  - If serial connection is not configured, then it is setup at 115200 baud

## General Information
  - Startup WiFi connect time = 3 seconds
  - WiFi switch over time (when one wifi was turned off and the other one is still on) = 5 seconds

  - Use following commands to generate a P-256 Elliptic Curve key pair:
    ```
    openssl ecparam -genkey -name prime256v1 -noout -out ec_private.pem
    openssl ec -in ec_private.pem -pubout -out ec_public.pem
    ```

  - Run the following command to get your private key
    ```
    openssl ec -in ec_private.pem -noout -text
    ```

  - Upload the public key to IoT Core under the respective gateway device.

## Types of standard messages

All system messages would range from -1 to negative infinity.
All user messages/device messages from 1 to positive infinity.

Type -1 is for updating WiFi credentials
```
{
  "t": -1,
  "ssid": "Qube",
  "pass": "Deep0asdkj",
  "index": 0
}
```

Type -2 for sending an OTA update
```
{
  "t": -2,
  "version": "1.2.0"
}
```

## Configurations
List of build variables that can be changed during the build phase
  - CORELIB_APP_COMM
  - CORELIB_IOTCORE
  - CORELIB_STATUS_LED
  - CORELIB_STORAGE
  - CORELIB_WIFI_MANAGER
  - CORELIB_FOTA

## Design

![corelib design](media/corelib-diagrams-net.png)

## Note
  - There is one problem which prevents compilation from happening. It says that **ESP8266HTTPClient.h**. One way to fix this problem is by including the header file in main project source file. For eg. Let's say FOTA.h includes ESP8266HTTPClient.h file. To fix this issue, in the project's main file (main.cpp) write `#include <FOTA.h>`. This somehow magically fixes the problem. Still don't know why this problem crops up.

  - Many a times project will not compile, during that time. Delete the `.pio/` folder and rebuild the project.
