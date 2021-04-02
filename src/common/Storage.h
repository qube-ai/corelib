#pragma once
#include <Arduino.h>
#include <EEPROM.h>

#include <string>

/**
 * @brief Number of bytes to initialize the EEPROM with
 * 
 */
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

namespace storage {

    /**
     * @brief Class to provide consistent API for accessing storage in ESP32 and ESP8266.
     * 
     */
    class AbstractedStorage {
       public:

       /**
        * @brief Initializes the EEPROM for storing and fetching data
        * 
        * @param size This is equal to EEPROM_SIZE
        * @note This needs to be called before anyother method is called
        */
        void begin(int size);

        /**
         * @brief Read a string starting at a given address
         * 
         * @param address Starting address of the string
         * @return std::string Read string from storage
         * 
         * ### Example
         * ~~~~~~~~~~~~~~.cpp
         * 
         * AbstractedStorage storage;
         * storage.begin(512);
         * 
         * std::string readData = storage.readString(10);
         * 
         * ~~~~~~~~~~~~~~
         * 
         */
        std::string readString(int address);

        /**
         * @brief Write string to storage
         * 
         * @param address Starting address where the string has to be written.
         * @param data String that has to be saved
         * @return true Saved successfully
         * @return false Not saved
         * 
         * @note 
         *   - This function internally calls commit. You **DON'T** need to call AbstractedStorage::commit() explicitly
         *   - Null character is automatically added to the end of the string.
         * 
         * ### Example
         * ~~~~~~~~~~~~~~.cpp
         * 
         * AbstractedStorage storage;
         * storage.begin(512);
         * storage.writeString(10, "Hello World");
         * 
         * ~~~~~~~~~~~~~~
         * 
         */
        bool writeString(int address, std::string data);

        /**
         * @brief Read string from storage
         * 
         * @param address Staring address from where the string needs to read
         * @param data Variable to which string data is copied
         * 
         * ### Example
         * Reading something that is stored at address 10 in EEPROM.
         * ~~~~~~~~~~~~~~.cpp
         * 
         * AbstractedStorage storage;
         * storage.begin(512);
         * 
         * char something[20] = "";
         * storage.readString(10, something);
         * 
         * ~~~~~~~~~~~~~~
         * 
         */
        void readString(int address, char *data);

        /**
         * @brief Read a byte from the given address
         * 
         * @param address Address from where the byte has to read
         * @return uint8_t Value of the byte read as an unsigned integer value.
         */
        uint8_t readByte(int address);

        void writeByte(int address, uint8_t data);

        /**
         * @brief Save the changes to flash (ESP8266 and ESP32).
         * @warning Not calling commit will not save any changes made on a memory location.
         */
        void commit();
    };

    extern AbstractedStorage storage;

    struct wifi_cred {
        char ssid[20];
        char password[20];
    };


    /**
     * @brief Initialize storage
     * @note This function needs to be called before any other function.
     * 
     */
    void init();


    /**
     * @brief Get stored WiFi Credentials from storage
     * 
     * @param index Value ranges from 0 to 4
     * @return wifi_cred Return SSID and Pass in wifi_cred struct
     * 
     * ### Example
     * ~~~~~~~~~~~~~~~~~~~~~~~.cpp
     * 
     * wifi_cred c1 = storage::getWiFiCreds(0);
     * 
     * ~~~~~~~~~~~~~~~~~~~~~~~
     * 
     */
    wifi_cred getWiFiCreds(short index);
    

    /**
     * @brief Set WiFi credentials in sotrage
     * 
     * @param index Index at which you want to store it (0 to 4)
     * @param ssid WiFi AP SSID as string
     * @param pass WiFi AP password as string
     * 
     * ### Example
     * ~~~~~~~~~~~~~~~~~~~~~~~.cpp
     * 
     * storage::setWiFiCreds(0, "Qube", "Deepwaves007");
     * 
     * ~~~~~~~~~~~~~~~~~~~~~~~
     */
    void setWiFiCreds(short index, std::string ssid, std::string pass);


    /**
     * @brief Set Google Cloud IoT core project ID to storage
     * 
     * @param project_id Google Cloud IoT Core Project ID
     * @return true Set successfully
     * @return false Unable to set. Something went wrong.
     * 
     * ### Example
     * ~~~~~~~~~~~~~~~~~~~~~~~.cpp
     * 
     * storage::setProjectID("podnet-switch");
     * 
     * ~~~~~~~~~~~~~~~~~~~~~~~
     * 
     */
    bool setProjectID(std::string project_id);
    

    /**
     * @brief Set Google Cloud project ID from storage
     * 
     * @param data Character array in which project ID will be copied.
     * 
     * ### Example
     * ~~~~~~~~~~.cpp
     * 
     * char project_id[20] = "";
     * storage::getProjectID(project_id);
     * 
     * ~~~~~~~~~~
     */
    void getProjectID(char *data);

    
    /**
     * @brief Set Google Cloud IoT Core location (default us-central1)
     * 
     * @param location String that describes the location of Google Cloud server
     * @return true Saved successfully
     * @return false Not saved
     * 
     * ### Example
     * ~~~~~~~~~~.cpp
     * 
     * storage::setLocation("us-central1");
     * 
     * ~~~~~~~~~~
     * 
     */
    bool setLocation(std::string location);


    /**
     * @brief Get Google Cloud IoT Core location from storage
     * 
     * @param data variable where location string is copied
     * 
     * ### Example
     * ~~~~~~~~~~.cpp
     * 
     * char location[20] = "";
     * storage::getLocation(location);
     * 
     * ~~~~~~~~~~
     * 
     */
    void getLocation(char *data);


    /**
     * @brief Set Google Cloud IoT Core registry ID to storage
     * 
     * @param registry_id Registry ID string
     * @return true Saved successfuly
     * @return false Not saved
     * 
     * ### Example
     * ~~~~~~~~~~.cpp
     * 
     * storage::setRegistryID("test-registry");
     * 
     * ~~~~~~~~~~
     * 
     */
    bool setRegistryID(std::string registry_id);


    /**
     * @brief Get Google Cloud IoT core registry ID from storage
     * 
     * @param data Registry ID is copied to this variable
     * 
     * ### Example
     * ~~~~~~~~~~.cpp
     * 
     * char registry_id[20] = "";
     * storage::getRegistryID(registry_id);
     * 
     * ~~~~~~~~~~
     * 
     */
    void getRegistryID(char *data);


    /**
     * @brief Set Google Cloud IoT Core device ID or gateway ID to storage
     * 
     * @param device_id Device ID to be stored
     * @return true Saved successfully
     * @return false Not saved
     * 
     * ### Example
     * ~~~~~~~~~~.cpp
     * 
     * storage::setDeviceID("SW2-30A-2");
     * 
     * ~~~~~~~~~~
     * 
     */
    bool setDeviceID(std::string device_id); 


    /**
     * @brief Get Google Cloud IoT Core device ID or gateway ID from storage
     * 
     * @param data Variable where device ID is copied
     * 
     * ### Example
     * ~~~~~~~~~~.cpp
     * 
     * char device_id[20] = "";
     * storage::getDeviceID(device_id);
     * 
     * ~~~~~~~~~~
     * 
     */
    void getDeviceID(char *data);

    /// Set current timezone to storage

    /**
     * @brief Set the timezone for the current device
     * 
     * @param timezone_sec Time to store in seconds
     * @return true Saved successfully
     * @return false Failed to save
     * 
     * ### Example
     * ~~~~~~~~~~.cpp
     * 
     * storage::setTimezone(19800);  // 5 hrs 30 minutes = 19800 seconds
     * 
     * ~~~~~~~~~~
     * 
     */
    bool setTimezone(long timezone_sec);

    /// Get current timezone from storage

    /**
     * @brief Get timezone of the device from storage
     * 
     * @param timezone_sec Variable to which timezone information is copied (in seconds)
     * 
     * ### Example
     * ~~~~~~~~~~.cpp
     * 
     * long timezone = 0;
     * storage::getTimezone(&timezone);  // 5 hrs 30 minutes = 19800 seconds
     * 
     * ~~~~~~~~~~
     * 
     */
    void getTimezone(long *timezone_sec);

    // Getter and Setter for last reset time for energy meter readings
    bool setLastResetTime(long timestamp);
    void getLastResetTime(long *timestamp);
}  // namespace storage
