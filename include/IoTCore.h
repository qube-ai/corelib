#pragma once

#include <Arduino.h>
#include "CloudIoTCoreMqtt.h"
#include <jled.h>

// To publish data to Cloud IoT Core
bool publishTelemetry(String data);
bool publishTelemetry(const char* data, int length);
bool publishTelemetry(String subfolder, String data);
bool publishTelemetry(String subfolder, const char* data, int length);
bool publishState(String data);

void setupCloudIoT();

void mqttLoop();

bool connectedToMqtt();
void logSomeShit();
extern char received_data[50];

#ifdef UNIT_TEST
extern String lastReceivedMessage;
extern bool messageAvailable;
#endif 