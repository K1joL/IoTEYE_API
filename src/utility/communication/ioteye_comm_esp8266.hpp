/*# MIT License

# Copyright (c) 2025 Shults Bogdan aka K1joL

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
*/

#ifndef IOTEYE_ESP8266_COMMUNICATION_HPP
#define IOTEYE_ESP8266_COMMUNICATION_HPP

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include "utility/ioteye_communication.hpp"
#include "utility/ioteye_config.hpp"
#include "utility/ioteye_debug.hpp"
#include "utility/ioteye_types.hpp"

namespace ioteye {
class ESP8266Communincation : public IIoTeyeCommunication {
public:
    ESP8266Communincation();
    ~ESP8266Communincation();
    Response sendData(HttpMethod method, const String& url,
                      const String& data = String()) override;
    void setLogger(DebugLogger* logger) override;
    void begin(const String& ssid, const String& password);
    void log(DebugLogger::LogLevel logLevel, const String& message);

private:
    HTTPClient m_httpClient;
    WiFiClient m_wifiClient;
    DebugLogger* m_logger = nullptr;
};

}  // namespace ioteye

extern ioteye::ESP8266Communincation ESP8266Comm;

#endif  // IOTEYE_ESP8266_COMMUNICATION_HPP