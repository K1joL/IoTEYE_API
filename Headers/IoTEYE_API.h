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

static std::string s_SERVER_URL = {"127.0.0.1"};
static std::string s_SERVER_PORT = {"8081"};
static std::string s_ENDPOINT_USERS = {"/user"};
static std::string s_ENDPOINT_PINS = {"/pins"};
static std::string s_ENDPOINT_DEVICES = {"/devices"};

#warning Before use requests you need to register user with RegisterUser() function
static std::string G_USERID{};
static std::string s_ENDPOINT_USERID{};

static std::unordered_map<std::string, uint64_t> s_devices;
static std::unordered_map<std::string, uint8_t> s_devicesStatus;

/*
 *   UserID пользователь получает в ответ на регистрацию
 *
 *
 *
 *
 */

/*
 *   TODO:
 *   make transfer authToken for methods
 *
 *
 *
 */

namespace ioteyeApi
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

    /*  Фукнция отправки запроса
     *   payload - request payload
     *   pResponse - pointer to cpr::Response if you want to get response
     *
     *   Returns:
     *   false - OK
     *   true - Error
     */
    bool sendRequest(uint8_t method,
                     const std::string &endpoint,
                     cpr::Payload &&payload = {},
                     cpr::Response *pResponse = nullptr);

    bool sendRequest(uint8_t method,
                     const std::string &endpoint,
                     cpr::Payload &payload,
                     cpr::Response *pResponse = nullptr);
    // Фукнция формирования запроса(пакета):
    // для команды создания нового пользователя системы ("ru")
    //
    //  Returns:
    //  - Empty string if something went wrong
    //  - Your user ID
    std::string registerNewUser(const std::string &customUserID = "");
    // Фукнция формирования запроса(пакета):
    // для команды создания нового устройства пользователя ("rd")
    bool registerNewDevice(const std::string &devName);
    // Фукнция формирования запроса(пакета):
    // для команды создания виртуального пина ("cp")
    bool createVirtualPin(const std::string &pinNumber, const std::string &dataType, const std::string &defaultData);
    // Фукнция формирования запроса(пакета):
    // для команды получения статуса девайса ("ds")
    //
    //   Returns:
    //   true - if something went wrong
    //   false - if request successfully processed
    //   update status of s_devicesStatus.at(devName)
    //
    bool getDeviceStatus(const std::string &devName);
    // Фукнция формирования запроса(пакета):
    //  для команды обновления значения пина ("up")
    bool writeVirtualPin(const std::string &pinNumber, const std::string &value);
    // Фукнция формирования запроса(пакета):
    //  для команды удаления пина ("dp")
    bool deleteVirtualPin(const std::string &pinNumber);
    // Фукнция формирования запроса(пакета):
    //  для команды получения данных с пина ("pv")
    std::string getVirtualPin(const std::string &pinNumber);

    // Функция для получения значения пина из ответа
    std::string getValue(const std::string &responseText, const std::string &key = "");

    // Функции для настройки пользоватлеьских эндопоинтов
    inline void setServerUrl(const std::string &serverUrl);
    inline void setServerPort(const std::string &serverPort);
    inline void setUsersEndpoint(const std::string &userEndpoint);
    inline void setPinsEndpoint(const std::string &pinsEndpoint);
    inline void setDeviceEndpoint(const std::string &deviceEndpoint);

    enum HTTP_METHOD
    {
        GET,
        POST,
        PUT,
        DELETE
    };
}

#endif //! IoTEYE_API_H