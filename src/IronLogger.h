#ifndef IRONLOGGER_H
#define IRONLOGGER_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <time.h>

class IronLogger {
public:
    // Constructor without Serial
    IronLogger(const char* serverIP, uint16_t serverPort, const char* hashValue, bool withNTP = true);

    // Constructor with Serial
    IronLogger(const char* serverIP, uint16_t serverPort, const char* hashValue, Stream& serial, bool withNTP = true);

    void begin(const char* ssid, const char* password);
    void sendLogMessage(const char* message, const char* level);

    void logInfo(const char* message) {
        sendLogMessage(message, "INFO");
    }

    void logWarn(const char* message) {
        sendLogMessage(message, "WARN");
    }

    void logError(const char* message) {
        sendLogMessage(message, "ERROR");
    }

    void logDebug(const char* message) {
        sendLogMessage(message, "DEBUG");
    }

private:
    const char* _serverIP;
    uint16_t _serverPort;
    const char* _hashValue;
    WiFiClient _client;
    bool _withNTP;
    Stream* _serial;
    void synchronizeTime();
    String escapeJson(const String& message);
};

#endif // IRONLOGGER_H
