#ifndef IoTEYE_API_H
#define IoTEYE_API_H

#include <iostream>
#include <string>
#include <cpr/cpr.h>
#include <unordered_map>

#ifndef SERVER_URL
#define SERVER_URL "127.0.0.1"
#endif //!SERVER_URL

#ifndef SERVER_PORT
#define SERVER_PORT "8081"
#endif //!SERVER_PORT

#ifndef ENDPOINT_USER
#define ENDPOINT_USER "/user"
#endif //!ENDPOINT_USER

// #ifndef CLIENT_API_MODE
//     extern const char *G_USERID;
//     static std::string s_ENDPOINT_USERID{G_USERID};
// #else
#ifndef USERID
#define USERID
    #warning Client api mode on. Before use requests you need to register user with RegisterUser() function
    static std::string G_USERID{};
    static std::string s_ENDPOINT_USERID{};
#endif //!USERID
// #endif // !CLIENT_API_MODE

#ifndef ENDPOINT_PINS
#define ENDPOINT_PINS "/pins"
#endif //!ENDPOINT_PINS

#ifndef ENDPOINT_DEVICE
#define ENDPOINT_DEVICE "/devices"
#endif //!ENDPOINT_DEVICE

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
/*  Фукнция отправки запроса
*   payload - request payload
*   pResponse - pointer to cpr::Response if you want to get response
*
*   Returns:
*   false - OK
*   true - Error
*/
    bool SendRequest(uint8_t method, cpr::Payload &payload, const std::string& endpoint = "", cpr::Response* pResponse = nullptr);
    //Фукнция формирования запроса(пакета):
    //для команды создания нового пользователя системы ("ru")
    //
    // Returns:
    // - Empty string if something went wrong
    // - Your user ID 
    // #ifdef CLIENT_API_MODE
    std::string RegisterNewUser(const std::string& customUserID = "");
    // #endif //!CLIENT_API_MODE
    //Фукнция формирования запроса(пакета):
    //для команды создания нового устройства пользователя ("rd")
    bool RegisterNewDevice(const std::string& devName);
    //Фукнция формирования запроса(пакета):
    //для команды создания виртуального пина ("cp")
    bool CreateVirtualPin(const std::string& pinNumber, const std::string& dataType, const std::string& defaultData);
    //Фукнция формирования запроса(пакета):
    //для команды получения статуса девайса ("ds")
    //
    //  Returns:
    //  true - if something went wrong
    //  false - if request successfully processed
    //  update status of s_devicesStatus.at(devName)
    //
    bool GetDeviceStatus(const std::string& devName);
    //Фукнция формирования запроса(пакета):
    // для команды обновления значения пина ("up")
    bool UpdateVirtualPin(const std::string& pinNumber, const std::string& value);
    //Фукнция формирования запроса(пакета):
    // для команды удаления пина ("dp")
    bool DeleteVirtualPin(const std::string& pinNumber);
    //Фукнция формирования запроса(пакета):
    // для команды получения данных с пина ("pv")
    std::string GetVirtualPin(const std::string& pinNumber);

    // Функция для получения значения пина из ответа 
    std::string GetValue(const std::string& responseText, const std::string& key = "");

    enum HTTP_METHOD
    {
        GET,
        POST,
        PUT,
        DELETE
    };
}

#endif //!IoTEYE_API_H