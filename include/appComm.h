#pragma once
#include <Arduino.h>

void setupAppCommunication();
void checkForAppCommMessages();
void sendAppCommMessage(String data);