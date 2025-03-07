#ifndef IOTEYE_ESP8266_COMMUNICATION_HPP
#define IOTEYE_ESP8266_COMMUNICATION_HPP

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include "utility/ioteye_communication.hpp"
#include "utility/ioteye_debug.hpp"
#include "utility/ioteye_types.hpp"
#include "utility/ioteye_config.hpp"

namespace ioteye {
class ESP8266Communincation : public IIoTeyeCommunication {
public:
    ESP8266Communincation();
    ~ESP8266Communincation();
    Response sendData(HttpMethod method, const String& url,
                      const String& data = String()) override;
    void begin(const String& ssid, const String& password);

private:
    HTTPClient m_httpClient;
    WiFiClient m_wifiClient;
};

}  // namespace ioteye

extern ioteye::ESP8266Communincation ESP8266Comm;

#endif  // IOTEYE_ESP8266_COMMUNICATION_HPP