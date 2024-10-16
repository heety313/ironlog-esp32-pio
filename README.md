# IronLog ESP32 Library
## Prerequisites

Before using IronLogger, ensure you have the following:

1. rust installed and `cargo` available in your path (run cargo to check)
2. platformio installed or Arduino IDE

## Installation

1. Clone this repository or download the ZIP file.
2. Move the src/.cpp & .h files to your Arduino libraries folder.
3. Copy examples/example.cpp text to your main to try it out.
4. Dont forget to change the serverIP to your local IP.

## IronLog Server Setup

IronLogger requires the IronLog server to be running on your network. To set up the server:

1. Install Rust if you haven't already: [https://www.rust-lang.org/tools/install](https://www.rust-lang.org/tools/install)
2. Install IronLog using Cargo:
   ```
   cargo install ironlog
   ```
3. Run the IronLog server:
   ```
   ironlog --tcp-listener-ip 0.0.0.0
   ```
4. Access the log viewer in your web browser at `http://localhost:8000`

## Usage

Here's a basic example of how to use IronLogger in your ESP32 project:

```cpp
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
```