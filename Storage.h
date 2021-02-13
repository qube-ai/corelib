#pragma once
#include <string>
#include <EEPROM.h>
#include <Arduino.h>
#include <string>

#define EEPROM_SIZE 512

#define SSID0_START 0
#define PASS0_START 33

#define SSID1_START 66
#define PASS1_START 99

#define SSID2_START 132
#define PASS2_START 165

#define SSID3_START 198
#define PASS3_START 231

#define SSID4_START 264
#define PASS4_START 297

#define PROJECT_ID_START 330
#define LOCATION_START 363
#define REGISTRY_ID_START 396
#define DEVICE_ID_START 429

#define EEPROM_INIT_ADDR 455
#define EEPROM_INIT_CHECK_VALUE 139

#define TIMEZONE_START 458
#define LAST_RESET_TIME_START 463

#define AVAILABLE_SSIDS 5

namespace storage
{

    class AbstractedStorage
    {
    public:
        void begin(int size);

        std::string readString(int address);
        bool writeString(int address, std::string data);

        void readString(int address, char *data);

        uint8_t readByte(int address);
        void writeByte(int address, uint8_t data);

        void commit();
    };

    extern AbstractedStorage storage;

    struct wifi_cred
    {
        char ssid[20];
        char password[20];
    };

    void init();

    // Getters and Setters for WiFi credentials
    wifi_cred getWiFiCreds(short index);
    void setWiFiCreds(short index, std::string ssid, std::string pass);

    // Getters for Project ID
    bool setProjectID(std::string project_id);
    void getProjectID(char *data);

    // Getters for Location
    bool setLocation(std::string location);
    void getLocation(char *data);

    // Getter for registry ID
    bool setRegistryID(std::string registry_id);
    void getRegistryID(char *data);

    // Getters for Device ID
    bool setDeviceID(std::string device_id);
    void getDeviceID(char *data);

    // Getter and Setter for device timezone
    bool setTimezone(long timezone_sec);
    void getTimezone(long *timezone_sec);

    // Getter and Setter for last reset time for energy meter readings
    bool setLastResetTime(long timestamp);
    void getLastResetTime(long *timestamp);
} // namespace storage
