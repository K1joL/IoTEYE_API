#ifndef IoTEYE_API_H
#define IoTEYE_API_H

#include <string>
#include <cpr/cpr.h>
#include <unordered_map>

/**
 * To enable debug mode add define IoTeyeDEBUG to your CMakeLists.txt
 */
#ifdef IoTeyeDEBUG
#warning Debug mode on
#include <iostream>
#endif // !IoTeyeDEBUG

class IoTeye
{
public:
    IoTeye()
    {
    }

    IoTeye(const std::string &serverHost, const std::string &serverPort)
        : m_serverHost{serverHost},
          m_serverPort{serverPort}
    {
    }

    uint16_t sendRequest(uint8_t method,
                         const std::string &endpoint,
                         cpr::Payload &&payload = {});

    uint16_t sendRequest(uint8_t method,
                         const std::string &endpoint,
                         cpr::Payload &payload);

    //  HTTP Requests
    /// @brief HTTP Request to register new device
    /// @return token
    std::string registerNewDevice();
    /// @brief HTTP Request to get device status with a token
    /// @param token 
    /// @return Device status if request is successful, HTTP Code
    uint16_t getDeviceStatus(const std::string &token);
    /// @brief HTTP Request to delete device with a token
    /// @param token 
    /// @return HTTP Code
    uint16_t deleteDevice(const std::string &token);
    /// @brief HTTP Request to create virtual pin
    /// @param token 
    /// @param pinNumber 
    /// @param dataType 
    /// @param defaultData 
    /// @return HTTP Code
    uint16_t createVirtualPin(const std::string &token, const std::string &pinNumber, const std::string &dataType, const std::string &defaultData);
    /// @brief HTTP Request to write virtual pin
    /// @param token
    /// @param pinNumber 
    /// @param value 
    /// @return HTTP Code
    uint16_t writeVirtualPin(const std::string &token, const std::string &pinNumber, const std::string &value);
    /// @brief HTTP Request to delete virtual pin
    /// @param token 
    /// @param pinNumber 
    /// @return HTTP Code
    uint16_t deleteVirtualPin(const std::string &token, const std::string &pinNumber);
    /// @brief HTTP Request to get virtual pin
    /// @param token 
    /// @param pinNumber 
    /// @return Pin value
    std::string getVirtualPin(const std::string &token, const std::string &pinNumber);

    //  Getters
    inline const cpr::Response &getLastResponse() { return m_lastResponse; }

    //  Setters
    inline void setServerHost(const std::string &serverHost)
    {
        m_serverHost = serverHost;
    }

    inline void setServerPort(const std::string &serverPort)
    {
        m_serverPort = serverPort;
    }

    inline void setPinsEndpoint(const std::string &pinsEndpoint)
    {
        m_endpointPins = pinsEndpoint;
    }

    inline void setDevicesEndpoint(const std::string &devicesEndpoint)
    {
        m_endpointDevices = devicesEndpoint;
    }

private:
    // Функция для получения значения пина из ответа
    std::string getValue(const std::string &responseText, const std::string &key = "");

public:
    enum HTTP_METHOD
    {
        GET,
        POST,
        PUT,
        DELETE
    };

private:
    std::string m_serverHost = {"127.0.0.1"};
    std::string m_serverPort = {"8081"};
    std::string m_endpointPins = {"/pins"};
    std::string m_endpointDevices = {"/devices"};
    cpr::Response m_lastResponse;
};

namespace IoTeyeDebug
{

    template <typename T>
    void debugMessage(const T &&mes)
    {
#ifdef IoTeyeDEBUG
        std::cout << mes;
#endif // !IoTeyeDEBUG
    }

    template <typename T>
    void debugMessage(const T &mes)
    {
#ifdef IoTeyeDEBUG
        std::cout << mes;
#endif // !IoTeyeDEBUG
    }
#define NEWLINE debugMessage("\n");
    template <typename T>
    void debugMessageln(const T &&mes)
    {
#ifdef IoTeyeDEBUG
        std::cout << mes;
        NEWLINE
#endif // !IoTeyeDEBUG
    }

    template <typename T>
    void debugMessageln(const T &mes)
    {
#ifdef IoTeyeDEBUG
        std::cout << mes;
        NEWLINE
#endif // !IoTeyeDEBUG
    }
}

#endif //! IoTEYE_API_H