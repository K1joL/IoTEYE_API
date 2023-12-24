#ifndef IoTEYE_API_H
#define IoTEYE_API_H

#ifndef SERVER_URL
#define SERVER_URL "127.0.0.1"
#endif //SERVER_URL

#ifndef SERVER_PORT
#define SERVER_PORT "8081"
#endif //SERVER_PORT

#ifndef ENDPOINT
#define ENDPOINT "/IoTEYE_PINS"
#endif //ENDPOINT

#ifndef CLIENT_API
#ifndef USERID
//#error
#endif //USERID
#endif //CLIENT_API

#include <cpr/cpr.h>
#include <iostream>
#include <string>

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

namespace IoTEYE_API
{
    // Фукнция отправки запроса
    bool SendRequest(uint8_t method, const cpr::Payload &payload, cpr::Response* pResponse = nullptr);
    //Фукнции формирования запроса(пакета):
    //для команды создания нового пользователя системы ("ru")
    //пока не знаю как сделать
    // bool RegisterNewUser();
    //для команды создания нового устройства пользователя ("rd")
    // bool RegisterNewDevice(const std::string& userID);
    //для команды создания виртуального пина ("cp")
    bool CreateVirtualPin(const std::string& pinNumber, const std::string& dataType);
    //для команды получения статуса девайса ("ds")
    bool GetDeviceStatus();
    // для команды обновления значения пина ("up")
    bool UpdateVirtualPin(const std::string& pinNumber, double data);
    bool UpdateVirtualPin(const std::string& pinNumber, int data);
    bool UpdateVirtualPin(const std::string& pinNumber, std::string& data);
    // для команды удаления пина ("dp")
    bool DeleteVirtualPin(const std::string& pinNumber);
    // для команды получения данных с пина ("pv")
    std::string GetVirtualPin(const std::string& pinNumber);

    enum HTTP_METHOD
    {
        GET,
        POST,
        PUT,
        DELETE
    };
}

#endif //IoTEYE_API_H

