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

#include "ioteye_comm_esp8266.hpp"

namespace ioteye {
ESP8266Communincation::ESP8266Communincation() {
}

ESP8266Communincation::~ESP8266Communincation() {
    if (!m_httpClient.connected())
        m_httpClient.end();
    if (m_wifiClient.connected() || (m_wifiClient.available() > 0))
        m_wifiClient.stop();
}

Response ESP8266Communincation::sendData(HttpMethod method, const char* url,
                                         const char* data) {
    m_httpClient.begin(m_wifiClient, url);
    m_httpClient.addHeader("Content-Type", "text/plain");

    HttpCode responseCode(0);
    switch (method) {
        case HttpMethod::POST:
            responseCode = m_httpClient.POST(data);
            break;
        case HttpMethod::GET:
            responseCode = m_httpClient.GET();
            break;
        case HttpMethod::PUT:
            responseCode = m_httpClient.PUT(data);
            break;
        case HttpMethod::DELETE:
            responseCode = m_httpClient.DELETE();
            break;
        default:
            return Response();
    }
    ioteye::Response response;
    if (!responseCode.isValid()) {
        response.body = m_httpClient.errorToString(responseCode);
        response.statusCode = responseCode;
        return response;
    }
    m_logger.logln(LogLevel::INFO, "Comm: ", responseCode);
    m_logger.logln(LogLevel::INFO, "Comm: ", m_httpClient.getString());
    response.body = m_httpClient.getString();
    response.statusCode = HttpCode(responseCode);
    m_httpClient.end();
    return response;
}
void ESP8266Communincation::setSerial(HardwareSerial* serial) {
    if (serial != nullptr)
        m_logger.setSerial(serial);
}
void ESP8266Communincation::begin(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    m_logger.logln(LogLevel::INFO, "Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        m_logger.log(LogLevel::INFO, ".");
    }
    m_logger.logln(LogLevel::INFO, "Connected to WiFi!");
}
}  // namespace ioteye

ioteye::ESP8266Communincation ESP8266Comm;
