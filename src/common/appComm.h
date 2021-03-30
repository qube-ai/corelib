#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "ArduinoJson.h"
#include "messageHandler.h"

namespace appcomm {
    void setupAppCommunication();
    void checkForAppCommMessages();
    void sendAppCommMessage(String data);
}  // namespace appcomm
