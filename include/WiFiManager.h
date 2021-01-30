#pragma once

enum STATES
{
    NOT_CONNECTED = 0,
    CONNECTED,
    INTERNET
};

void setupWiFi();
void reconnectWiFi(bool log);

