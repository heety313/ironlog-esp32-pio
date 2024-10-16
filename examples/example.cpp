#include "IronLogger.h"

// Replace these with your network credentials
const char* ssid = "my_wifi_name";
const char* password = "my_password";

// Replace these with your server details
const char* serverIP = "192.168.1.XXX"; // Server IP address
const uint16_t serverPort = 5000;      // Server port number

const char* hash_value = "NICE1"; // Replace with your hash value

IronLogger logger((const char *)serverIP, serverPort, hash_value, Serial, (bool)false);

void setup() {
    Serial.begin(115200);
    logger.begin(ssid, password);
}

void loop() {
    //logger.sendLogMessage("This is a test log message", "INFO");

    logger.logError("Yikes");
    logger.logWarn("not good");
    logger.logInfo("This is a longer message, that is longer than a standard packet in order to test the integrity of the logging frameworks, lets see how it performs under stress.");
    delay(6000);
}
