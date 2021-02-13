#ifdef ESP32

    #include "FOTA_esp32.h"

String host = "storage.googleapis.com";
WiFiClient client;

// Variables to validate response from GCP
long contentLength = 0;
bool isValidContentType = false;

// Utility to extract header value from headers
String getHeaderValue(String header, String headerName) {
    return header.substring(strlen(headerName.c_str()));
}

void fota::performOTAUpdate(String version) {
    Serial.println("Starting OTA operation.");

    // Check if the version number is correct.
    if (version.length() == 0) {
        Serial.println("Please specify a version number.");
        return;
    }

    // Prepare the firmware and checksum download URL
    String firmwareRelativeURL = "/ota-server/" + version + "/firmware.bin";
    String checksumRelativeURL = "/ota-server/" + version + "/checksum.txt";
    Serial.println("Firmware relative download URL: " + firmwareRelativeURL);
    Serial.println("Checksum URL: " + checksumRelativeURL);

    // https://storage.googleapis.com/ota-server/1.0.0-test/firmware.bin

    if (client.connect(host.c_str(), 80)) {
        Serial.println("Connected to OTA Server");

        client.print(String("GET ") + firmwareRelativeURL + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" + "Cache-Control: no-cache\r\n" +
                     "Connection: close\r\n\r\n");

        // Wait for the response from server
        unsigned long timeout = millis();
        while (client.available() == 0) {
            if (millis() - timeout > 5000) {
                Serial.println("Client Timeout !");
                client.stop();
                return;
            }
        }

        // Response Structure from GCP Bucket
        /*
            HTTP/1.1 200 OK
            X-GUploader-UploadID:
           ABg5-UxcEyzU5lPixHuyElJO_tWYg07KwjBSp5mcrapeeG2FviDUmsHYDmKP7cPTsJMH6Y9BizGpA9AcVGnGGu6DVpI
            Expires: Fri, 16 Oct 2020 16:38:54 GMT
            Date: Fri, 16 Oct 2020 15:38:54 GMT
            Cache-Control: public, max-age=3600
            Last-Modified: Fri, 16 Oct 2020 15:38:16 GMT
            ETag: "8cf143c0b51a9e2c790126005cf2766f"
            x-goog-generation: 1602862696015592
            x-goog-metageneration: 1
            x-goog-stored-content-encoding: identity
            x-goog-stored-content-length: 895744
            Content-Type: application/octet-stream
            x-goog-hash: crc32c=YtwUCg==
            x-goog-hash: md5=jPFDwLUanix5ASYAXPJ2bw==
            x-goog-storage-class: STANDARD
            Accept-Ranges: bytes
            Content-Length: 895744
            Server: UploadServer
            Connection: close

            {{ BIN FILE CONTENTS }}
        */

        while (client.available()) {
            // read line till /n
            String line = client.readStringUntil('\n');
            // remove space, to check if the line is end of headers
            line.trim();

            // if the the line is empty,
            // this is end of headers
            // break the while and feed the
            // remaining `client` to the
            // Update.writeStream();
            if (!line.length()) {
                // headers ended
                break;  // and get the OTA started
            }

            // Check if the HTTP Response is 200
            // else break and Exit Update
            if (line.startsWith("HTTP/1.1")) {
                if (line.indexOf("200") < 0) {
                    Serial.println(
                        "Got a non 200 status code from server. Exiting OTA "
                        "Update.");
                    break;
                }
            }

            // extract headers here
            // Start with content length
            if (line.startsWith("Content-Length: ")) {
                contentLength =
                    atol((getHeaderValue(line, "Content-Length: ")).c_str());
                Serial.println("Got " + String(contentLength) +
                               " bytes from server");
            }

            // Next, the content type
            if (line.startsWith("Content-Type: ")) {
                String contentType = getHeaderValue(line, "Content-Type: ");
                Serial.println("Got " + contentType + " payload.");
                if (contentType == "application/octet-stream") {
                    isValidContentType = true;
                }
            }
        }
    } else {
        Serial.println("Connection to " + host +
                       " failed. Please check your network.");
    }

    // check contentLength and content type
    if (contentLength && isValidContentType) {
        // Check if there is enough space for an OTA Update
        bool canBegin = Update.begin(contentLength);

        // If yes, begin
        if (canBegin) {
            // Light up the test led before performing OTA
            pinMode(2, OUTPUT);
            digitalWrite(2, HIGH);

            Serial.println(
                "Begin OTA. This may take 2 - 5 mins to complete. Things might "
                "be quite for a while.. Patience!");
            // No activity would appear on the Serial monitor
            // So be patient. This may take 2 - 5mins to complete

            size_t written = Update.writeStream(client);

            if (written == contentLength) {
                Serial.println("Written : " + String(written) +
                               " successfully");
            } else {
                Serial.println("Written only : " + String(written) + "/" +
                               String(contentLength) + ". Retry?");
                // retry??
                // execOTA();
            }

            if (Update.end()) {
                Serial.println("OTA done!");
                if (Update.isFinished()) {
                    Serial.println("Update successfully completed. Rebooting.");
                    ESP.restart();
                } else {
                    Serial.println(
                        "Update not finished? Something went wrong!");
                }
            } else {
                Serial.println("Error Occurred. Error #: " +
                               String(Update.getError()));
            }
        } else {
            // not enough space to begin OTA
            // Understand the partitions and
            // space availability
            Serial.println("Not enough space to begin OTA");
            client.flush();
        }
    } else {
        Serial.println("There was no content in the response");
        client.flush();
    }
}

#endif