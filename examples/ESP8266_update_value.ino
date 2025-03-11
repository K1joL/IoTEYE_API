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