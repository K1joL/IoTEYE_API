#ifndef IOTEYE_ESP8266_H
#define IOTEYE_ESP8266_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <payload.h>
#include <IoTEYELiterals.h>

class IoTeye
{
private:
    // Server configuration
    String m_serverHOST = "127.0.0.1";
    String m_serverPORT = "8081";
    String m_endpointPins = "/pins";
    String m_endpointDevices = "/devices";
    String m_serverURL;
    String m_token;

    // latest error showing what went wrong while processing the request
    uint8_t m_lastHttpCode = 0;
    String m_lastResponsePayload;

    WiFiClient client;
    HTTPClient http;

    HardwareSerial *m_serial = nullptr;

public:
    enum HTTP_METHOD
    {
        GET,
        POST,
        PUT,
        DELETE
    };

public:
    IoTeye()
    {
    }
    // Функции для настройки пользовательских эндопоинтов
    void setServerUrl(const String &serverUrl)
    {
        m_serverHOST = serverUrl;
    }
    void setServerPort(const String &serverPort)
    {
        m_serverPORT = serverPort;
    }
    void setPinsEndpoint(const String &pinsEndpoint)
    {
        m_endpointPins = pinsEndpoint;
    }
    void setDeviceEndpoint(const String &deviceEndpoint)
    {
        m_endpointDevices = deviceEndpoint;
    }

// debug messages
#ifdef IOTEYE_DEBUG
    void debugInit(HardwareSerial *serial)
    {
        m_serial = serial;
    }
#endif // !IOTEYE_DEBUG
    template <typename T>
    void debugMessage(const T &&mes)
    {
#ifdef IOTEYE_DEBUG
        if (m_serial != nullptr)
            m_serial->print(mes);
#endif // !IOTEYE_DEBUG
    }
    template <typename T>
    void debugMessage(const T &mes)
    {
#ifdef IOTEYE_DEBUG
        if (m_serial != nullptr)
            m_serial->print(mes);
#endif // !IOTEYE_DEBUG
    }
#ifdef IOTEYE_DEBUG
#define NEWLINE m_serial->println();
#else
#define NEWLINE ;
#endif // !IOTEYE_DEBUG

    template <typename T>
    void debugMessageln(const T &&mes)
    {
#ifdef IOTEYE_DEBUG
        debugMessage(mes);
        NEWLINE
#endif // !IoTeyeDEBUG
    }

    template <typename T>
    void debugMessageln(const T &mes)
    {
#ifdef IOTEYE_DEBUG
        debugMessage(mes);
        NEWLINE
#endif // !IoTeyeDEBUG
    }

    uint8_t getError() { return m_lastHttpCode; }

    // method declarations
    void begin(const char *token,
               const char *ssid,
               const char *pass,
               const char *domain,
               const char *port);
    uint16_t sendRequest(uint8_t method,
                         const String &endpoint,
                         const ioteye::Payload &payload = {});
    void run();
    void printError();
    /// @brief HTTP Request to create virtual pin
    /// @param pinNumber 
    /// @param dataType 
    /// @param defaultData 
    /// @return HTTP Code
    uint16_t createVirtualPin(const String &pinNumber, const String &dataType, const String &defaultData);
    uint16_t createVirtualPin(const String &pinNumber, const String &dataType, const int &defaultData);
    uint16_t createVirtualPin(const String &pinNumber, const String &dataType, const double &defaultData);
    /// @brief HTTP Request to write virtual pin
    /// @param pinNumber 
    /// @param value 
    /// @return HTTP Code
    uint16_t writeVirtualPin(const String &pinNumber, const String &value);
    /// @brief HTTP Request to delete virtual pin
    /// @param pinNumber 
    /// @return HTTP Code
    uint16_t deleteVirtualPin(const String &pinNumber);
    /// @brief HTTP Request to get virtual pin
    /// @param pinNumber 
    /// @return Pin value
    String getVirtualPin(const String &pinNumber);
    /// @brief HTTP Request to get device status with a token
    /// @param otherToken 
    /// @return Device status if request is successful, HTTP Code if unsuccessful
    uint16_t getDeviceStatus(const String &otherToken = "");
    /// @brief HTTP Request to ping device on the server
    /// @return HTTP Code
    uint16_t updateDeviceStatus();

    String getValue(const String &responseText, const String &key = "");
    String getResponse();
};
// method implementations
void IoTeye::begin(const char *token,
                   const char *ssid,
                   const char *pass,
                   const char *domain,
                   const char *port)
{
    this->m_token = token;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    m_serial->println();
    m_serial->print("Connected! IP address: ");
    m_serial->println(WiFi.localIP());
    this->m_serverHOST = domain;
    this->m_serverPORT = port;
    this->m_serverURL = m_serverHOST + ":" + m_serverPORT;
}

void IoTeye::run()
{
    updateDeviceStatus();
    delay(50);
}

void IoTeye::printError()
{
    if (m_lastHttpCode <= 0)
    {
        m_serial->print("Server error: ");
        m_serial->println((m_lastResponsePayload));
        m_serial->print("HTTPClient error: ");
        m_serial->println(http.errorToString(m_lastHttpCode));
    }
}

uint16_t IoTeye::sendRequest(uint8_t method,
                             const String &endpoint,
                             const ioteye::Payload &payload)
{
    String url = "http://";
    url += m_serverURL;
    if (!endpoint.isEmpty())
        url += endpoint;
    else
        url += '/';
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    switch (method)
    {
    case POST:
        m_lastHttpCode = http.POST(payload.GetArgsString());
        break;
    case GET:
        m_lastHttpCode = http.GET();
        break;
    case PUT:
        m_lastHttpCode = http.PUT(payload.GetArgsString());
        break;
    case DELETE:
        m_lastHttpCode = http.DELETE();
        break;
    default:
        debugMessageln("Error: unknown method!");
        return 0;
    }
    if (m_lastHttpCode <= 0)
    {
        debugMessage("Error http code: ");
        debugMessageln(m_lastHttpCode);
        debugMessage("Error: ");
        debugMessageln(http.errorToString(m_lastHttpCode));
        return m_lastHttpCode;
    }
    m_lastResponsePayload = http.getString();
    debugMessage("Response: ");
    debugMessageln(m_lastResponsePayload);
    http.end();
    return m_lastHttpCode;
}

uint16_t IoTeye::createVirtualPin(const String &pinNumber,
                                  const String &dataType,
                                  const int &defaultData)
{
    return createVirtualPin(pinNumber, dataType, String(defaultData));
}
uint16_t IoTeye::createVirtualPin(const String &pinNumber,
                                  const String &dataType,
                                  const double &defaultData)
{
    return createVirtualPin(pinNumber, dataType, String(defaultData));
}

uint16_t IoTeye::createVirtualPin(const String &pinNumber,
                                  const String &dataType,
                                  const String &defaultData)
{
    ioteye::Payload p{{"token", m_token}};
    p.Add({"cmd", "cp"});
    p.Add({"pinNumber", pinNumber});
    p.Add({"dataType", dataType});
    p.Add({"defValue", defaultData});

    String endpoint{m_endpointDevices};
    endpoint += m_endpointPins;

    return sendRequest(POST, endpoint, p);
}

uint16_t IoTeye::writeVirtualPin(const String &pinNumber, const String &value)
{
    ioteye::Payload p{{"token", m_token}};
    p.Add({"cmd", "up"});
    p.Add({"pinNumber", pinNumber});
    p.Add({"value", value});

    String endpoint{m_endpointDevices};
    endpoint += m_endpointPins;

    return sendRequest(PUT, endpoint, p);
}

uint16_t IoTeye::deleteVirtualPin(const String &pinNumber)
{
    String endpoint{m_endpointDevices};
    endpoint += "/" + m_token;
    endpoint += m_endpointPins;
    endpoint += "/" + pinNumber;
    endpoint += "/dp";

    return sendRequest(DELETE, endpoint);
}

String IoTeye::getVirtualPin(const String &pinNumber)
{
    String endpoint{m_endpointDevices};
    endpoint += "/" + m_token +
                m_endpointPins +
                "/" + pinNumber +
                "/pv";

    if (sendRequest(GET, endpoint) == HTTP_CODE_OK)
        return getValue(m_lastResponsePayload, "PinValue");
    return String("Error");
}

uint16_t IoTeye::getDeviceStatus(const String &otherToken)
{
    String token{};
    if (otherToken.isEmpty())
        token = m_token;
    else
        token = otherToken;

    String endpoint{m_endpointDevices};
    endpoint += "/" + m_token +
                "/ds";

    bool status = 0;
    if (sendRequest(GET, endpoint) == HTTP_CODE_OK)
    {
        status = (getValue(http.getString(), "devStatus")[0] + 48);

        debugMessage("Device is ");
        debugMessageln(((status) ? "Online" : "Offline!"));
        return status;
    }

    return m_lastHttpCode;
}

uint16_t IoTeye::updateDeviceStatus()
{
    String endpoint{m_endpointDevices};
    endpoint += "/" + m_token +
                "/us";
    return sendRequest(GET, endpoint);
}

String IoTeye::getValue(const String &responseText, const String &key)
{
    String value{responseText.substring(responseText.indexOf(key + "=") + key.length() + 1)};
    return value;
}

String IoTeye::getResponse()
{
    return this->m_lastResponsePayload;
}

extern IoTeye eye;

#endif // !IOTEYE_ESP8266_H