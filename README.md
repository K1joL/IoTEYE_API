# IoTeye Arduino Library

IoTeye Arduino Library is a versatile library for interacting with the IoTeye server, designed specifically for controller-based projects. This library allows you to manage virtual pins, communicate with the IoTeye server, and perform various operations to control and monitor IoT devices.

## Features

- **Virtual Pin Management**: Easily create, update, and delete virtual pins.
- **Device Status Management**: Monitor and update the status of your IoT devices.
- **Customizable**: Adaptable to fit different use cases and hardware configurations. 
    - **(Only ESP8266 currently)**

## Getting Started

### Prerequisites

- Arduino IDE installed
- An Arduino-compatible board (e.g., ESP8266)
- IoTeye device token

### Installation

1. **Download the library from releases**:
   - [Latest Release (click)](https://github.com/K1joL/IoTEYE_API/releases/latest)

2. **Install the library**:
   - Open the Arduino IDE.
   - Go to `Sketch` -> `Include Library` -> `Add .ZIP Library...`.
   - Select the `IoTEYE_API-*.*.*.zip` file that you downloaded.

### Usage Example

Below is an example of how to use the `IoTeye API Library` to set up a simple IoT device that manages virtual pins and communicates with the IoTeye server.

```cpp ESP8266_update_value.ino
// include specific for your board file
#include <IoTeyeEsp8266.hpp>
// Define your credentials
const char* token = "****.****.****";
const char* ssid = "****";
const char* pass = "****";
// Your server's ip
const char* ip = "localhost:8080";

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Set your options
  IoTeyeSettings.setSSID(ssid)
    .setPassword(pass)
    .setToken(token)
    .setServerUrl(ip)
  //.setLogger(&Serial) // if you needed to Debug something internal
    .begin(); // After begin() you can use 'iotEye' object

  // Create a virtual pin
  int defaultValue = 0;
  // Use the VP(N) preprocessor directive where N is the pin number
  iotEye.createVirtualPin(VP(1), VPINT, defaultValue);

}

void loop() {
  // Update the device status every 5 seconds
  iotEye.run(5000);
  // iotEye.run(); // Update the device status with default interval

  // Get the value of the virtual pin
  String value = iotEye.getVirtualPin(VP(1));
  Serial.println("Virtual Pin Value: " + value);

  // Write a value to the virtual pin
  iotEye.writeVirtualPin(VP(1), value + 10);          // for int
  // iotEye.writeVirtualPin(VP(1), value + "10");     // for string
  // iotEye.writeVirtualPin(VP(1), value + 10.0);     // for double

  // Don't use delays if you want everything to work properly
  // delay(10000);
}
```

In this example:
- We we set up and initialize an `IoTeye` object
- We create a virtual pin and write a value to it.
- We periodically update the device status, read the value of the virtual pin and update it.

## License

This project is licensed under the MIT License - see the [COPYING](COPYING) file for details.