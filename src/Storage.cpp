#include "Storage.h"
#include <EEPROM.h>
#include <Arduino.h>
#include <string>

AbstractedStorage storage;

int WIFI_CREDS_ADDR[5][2] = {
    {SSID0_START, PASS0_START},
    {SSID1_START, PASS1_START},
    {SSID2_START, PASS2_START},
    {SSID3_START, PASS3_START},
    {SSID4_START, PASS4_START}};

void AbstractedStorage::begin(int size)
{
    EEPROM.begin(size);
}

void AbstractedStorage::readString(int address, char *data)
{
    char temp[32];
    EEPROM.get(address, temp);
    strcpy(data, temp);
}

std::string AbstractedStorage::readString(int address)
{
    char temp[32];
    EEPROM.get(address, temp);
    std::string final_string(temp);
    return final_string;
}

bool AbstractedStorage::writeString(int address, std::string data)
{
    short length_of_array = data.length();
    const char *data_array = data.c_str();

    int i = 0;
    for (i = 0; i < length_of_array; i++)
    {
        EEPROM.write(address + i, data_array[i]);
    }
    EEPROM.write(address + i, '\0');
    bool a = EEPROM.commit();
    delay(200);
    return a;
}

uint8_t AbstractedStorage::readByte(int address)
{
    return EEPROM.read(address);
}

void AbstractedStorage::writeByte(int address, uint8_t data)
{
    EEPROM.write(address, data);
}

void AbstractedStorage::commit()
{
    EEPROM.commit();
}

void initStorage()
{
#ifndef UNIT_TEST
    Serial.print("Initialising local storage...");
#endif

    storage.begin(EEPROM_SIZE);

    // Check for EEPROM Initialization
    if (storage.readByte(EEPROM_INIT_ADDR) == EEPROM_INIT_CHECK_VALUE)
    {
#ifndef UNIT_TEST
        Serial.println("EEPROM has already been initialized.");
#endif
    }
    else
    {
#ifndef UNIT_TEST
        Serial.println("EEPROM is not initialized.");
#endif
    }
}

wifi_cred getWiFiCreds(short index)
{
    wifi_cred temp;
    storage.readString(WIFI_CREDS_ADDR[index][0], temp.ssid);
    storage.readString(WIFI_CREDS_ADDR[index][1], temp.password);

    return temp;
}

void setWiFiCreds(short index, std::string ssid, std::string password)
{
    if (index >= 0 && index <= 4)
    {
        storage.writeString(WIFI_CREDS_ADDR[index][0], ssid);
        storage.writeString(WIFI_CREDS_ADDR[index][1], password);
        storage.commit();

#ifndef UNIT_TEST
        Serial.print("Saved new WiFi creds to local storage at SSID");
        Serial.print(index);
        Serial.print(" and PASS");
        Serial.print(index);
        Serial.println(".");
#endif
    }
    else
    {
#ifndef UNIT_TEST
        Serial.println("Invalid index specified for setting WiFi creds");
#endif
    }
}

bool setProjectID(std::string project_id)
{
    return storage.writeString(PROJECT_ID_START, project_id);
}

void getProjectID(char *data)
{
    char t[32];
    EEPROM.get(PROJECT_ID_START, t);
    strcpy(data, t);
}

bool setLocation(std::string location)
{
    return storage.writeString(LOCATION_START, location);
}

void getLocation(char *data)
{
    char t[32];
    EEPROM.get(LOCATION_START, t);
    strcpy(data, t);
}

bool setRegistryID(std::string registry_id)
{
    return storage.writeString(REGISTRY_ID_START, registry_id);
}

void getRegistryID(char *data)
{
    char t[32];
    EEPROM.get(REGISTRY_ID_START, t);
    strcpy(data, t);
}

bool setDeviceID(std::string device_id)
{
    return storage.writeString(DEVICE_ID_START, device_id);
}

void getDeviceID(char *data)
{
    char t[32];
    EEPROM.get(DEVICE_ID_START, t);
    strcpy(data, t);
}

bool setTimezone(long timezone_sec)
{
    EEPROM.put(TIMEZONE_START ,timezone_sec);
    bool a = EEPROM.commit();
    delay(200);
    return a;
}

void getTimezone(long *timezone_sec)
{
    EEPROM.get(TIMEZONE_START, *timezone_sec);
}
