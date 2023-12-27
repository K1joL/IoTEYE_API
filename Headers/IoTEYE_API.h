#ifndef IoTEYE_API_H
#define IoTEYE_API_H

#include <iostream>
#include <string>
#include <cpr/cpr.h>

#ifndef SERVER_URL
#define SERVER_URL "127.0.0.1"
#endif //SERVER_URL

#ifndef SERVER_PORT
#define SERVER_PORT "8081"
#endif //SERVER_PORT

#ifndef ENDPOINT_USER
#define ENDPOINT_USER "/user"
#endif //ENDPOINT_USER

// #ifndef CLIENT_API_MODE
    extern const char *G_USERID;
// #else
//     #warning Client api mode on. Global variable 'const char* G_USERID' is not defined!
//     #warning Before use requests you need to register user with RegisterUser() function
//     static const char *G_USERID = "/puk";
// #endif // CLIENT_API_MODE

#ifndef ENDPOINT_PINS
#define ENDPOINT_PINS "/pins"
#endif //ENDPOINT_PINS

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
    //Фукнции формирования запроса(пакета):
    //для команды создания нового пользователя системы ("ru")
    //пока не знаю как сделать
    // bool RegisterNewUser();
    //для команды создания нового устройства пользователя ("rd")
    // bool RegisterNewDevice(const std::string& userID);
    //для команды создания виртуального пина ("cp")
    bool CreateVirtualPin(const std::string& pinNumber, const std::string& dataType, const std::string& defaultData);
    //для команды получения статуса девайса ("ds")
    bool GetDeviceStatus();
    // для команды обновления значения пина ("up")
    bool UpdateVirtualPin(const std::string& pinNumber, const std::string& value);
    // для команды удаления пина ("dp")
    bool DeleteVirtualPin(const std::string& pinNumber);
    // для команды получения данных с пина ("pv")
    std::string GetVirtualPin(const std::string& pinNumber);

    // Функция для получения значения пина из ответа 
    std::string GetValue(const std::string& responseText);

    enum HTTP_METHOD
    {
        GET,
        POST,
        PUT,
        DELETE
    };
}

#endif //IoTEYE_API_H

