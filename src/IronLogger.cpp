#include "IronLogger.h"

IronLogger::IronLogger(const char* serverIP, uint16_t serverPort, const char* hashValue, bool withNTP)
    : _serverIP(serverIP), _serverPort(serverPort), _hashValue(hashValue), _withNTP(withNTP), _serial(nullptr) {}

IronLogger::IronLogger(const char* serverIP, uint16_t serverPort, const char* hashValue, Stream& serial, bool withNTP)
    : _serverIP(serverIP), _serverPort(serverPort), _hashValue(hashValue), _withNTP(withNTP), _serial(&serial) {}

void IronLogger::begin(const char* ssid, const char* password) {
    if (_serial) {
        delay(1000);
        _serial->println("Connecting to Wi-Fi...");
    }

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        if (_serial) {
            _serial->print(".");
        }
    }

    if (_serial) {
        _serial->println("\nWi-Fi connected");
        _serial->print("IP address: ");
        _serial->println(WiFi.localIP());
    }

    if (_withNTP) {
        synchronizeTime();
    }
}

void IronLogger::synchronizeTime() {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    if (_serial) {
        _serial->println("Synchronizing time...");
    }
    while (time(nullptr) < 100000) {
        delay(100);
        if (_serial) {
            _serial->print(".");
        }
    }
    if (_serial) {
        _serial->println("\nTime synchronized");
    }
}

void IronLogger::sendLogMessage(const char* message, const char* level) {
    if (!_client.connect(_serverIP, _serverPort)) {
        if (_serial) {
            _serial->println("Connection to log server failed");
        }
        return;
    }

    String jsonMessage;

    if (_withNTP) {
        time_t now = time(nullptr);
        struct tm timeinfo;
        gmtime_r(&now, &timeinfo);

        char timestamp[30];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);

        jsonMessage = String("{\"timestamp\":\"") + timestamp +
                      "\",\"level\":\"" + level +
                      "\",\"message\":\"" + escapeJson(message) +
                      "\",\"target\":\"ESP32_Device\"" +
                      ",\"module_path\":\"\"" +
                      ",\"file\":\"" + __FILE__ +
                      "\",\"line\":" + String(__LINE__) +
                      ",\"hash\":\"" + _hashValue + "\"}";
    } else {
        jsonMessage = String("{\"level\":\"") + level +
                      "\",\"message\":\"" + escapeJson(message) +
                      "\",\"target\":\"ESP32_Device\"" +
                      ",\"module_path\":\"\"" +
                      ",\"file\":\"" + __FILE__ +
                      "\",\"line\":" + String(__LINE__) +
                      ",\"hash\":\"" + _hashValue + "\"}";
    }

    _client.print(jsonMessage);
    _client.print('\n');

    if (_serial) {
        _serial->print("Sent log message: ");
        _serial->println(jsonMessage);
    }

    _client.stop();
}

String IronLogger::escapeJson(const String& message) {
    String escaped = message;
    escaped.replace("\\", "\\\\");
    escaped.replace("\"", "\\\"");
    escaped.replace("/", "\\/");
    escaped.replace("\b", "\\b");
    escaped.replace("\f", "\\f");
    escaped.replace("\n", "\\n");
    escaped.replace("\r", "\\r");
    escaped.replace("\t", "\\t");
    return escaped;
}
