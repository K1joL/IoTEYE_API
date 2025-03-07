#include "ioteye_esp8266.hpp"

namespace ioteye {
ESP8266Communincation::ESP8266Communincation() {
}

ESP8266Communincation::~ESP8266Communincation() {
    if(!m_httpClient.connected())
        m_httpClient.end();
    if(m_wifiClient.connected() || (m_wifiClient.available() > 0))
        m_wifiClient.stop();
}

Response ESP8266Communincation::sendData(HttpMethod method, const String& url,
                                         const String& data) {
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
    LibLogger.log(DebugLogger::INFO, "client end");
    LibLogger.log(DebugLogger::INFO, String(responseCode));
    LibLogger.log(DebugLogger::INFO, m_httpClient.getString());
    response.body = m_httpClient.getString();
    response.statusCode = HttpCode(responseCode);
    m_httpClient.end();
    return response;
}
void ESP8266Communincation::begin(const String& ssid, const String& password) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    LibLogger.log(DebugLogger::INFO, "Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        LibLogger.log(DebugLogger::INFO, ".");
    }
    LibLogger.log(DebugLogger::INFO, "\nConnected to WiFi!");
}
}  // namespace ioteye

ioteye::ESP8266Communincation ESP8266Comm;
