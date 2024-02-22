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

/*
 *   UserID пользователь получает в ответ на регистрацию
 */

class IoTeye
{
public:
    IoTeye()
    {
    }
    /*  Фукнция отправки запроса
     *   payload - request payload
     *   pResponse - pointer to cpr::Response if you want to get response
     *
     *   Returns:
     *   false - OK
     *   true - Error
     */
    uint16_t sendRequest(uint8_t method,
                     const std::string &endpoint,
                     cpr::Payload &&payload = {},
                     cpr::Response *pResponse = nullptr);

    uint16_t sendRequest(uint8_t method,
                     const std::string &endpoint,
                     cpr::Payload &payload,
                     cpr::Response *pResponse = nullptr);

    // Фукнция формирования запроса(пакета):
    // для команды создания нового устройства пользователя ("rd")
    std::string registerNewDevice();
    // Фукнция формирования запроса(пакета):
    // для команды создания виртуального пина ("cp")
    uint16_t createVirtualPin(const std::string &token, const std::string &pinNumber, const std::string &dataType, const std::string &defaultData);
    // Фукнция формирования запроса(пакета):
    // для команды получения статуса девайса ("ds")
    //
    //   Returns:
    //   true - if something went wrong
    //   false - if request successfully processed
    //   update status of s_devicesStatus.at(devName)
    //
    uint16_t getDeviceStatus(const std::string &token);
    // Фукнция формирования запроса(пакета):
    //  для команды обновления значения пина ("up")
    uint16_t writeVirtualPin(const std::string &token, const std::string &pinNumber, const std::string &value);
    // Фукнция формирования запроса(пакета):
    //  для команды удаления пина ("dp")
    uint16_t deleteVirtualPin(const std::string &token, const std::string &pinNumber);
    // Фукнция формирования запроса(пакета):
    //  для команды получения данных с пина ("pv")
    std::string getVirtualPin(const std::string &token, const std::string &pinNumber);

    // Функция для получения значения пина из ответа
    std::string getValue(const std::string &responseText, const std::string &key = "");

    // Функции для настройки пользоватлеьских эндопоинтов
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