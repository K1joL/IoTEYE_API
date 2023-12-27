#ifndef IoTEYE_API_HPP
#define IoTEYE_API_HPP

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

#ifndef CLIENT_API_MODE
    extern const char *G_USERID;
    static std::string s_ENDPOINT_USERID{G_USERID};
#else
    #warning Client api mode on. Before use requests you need to register user with RegisterUser() function
    std::string G_USERID{};
    static std::string s_ENDPOINT_USERID{};
#endif // !CLIENT_API_MODE

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
    #ifdef CLIENT_API_MODE
    std::string RegisterNewUser(const std::string& customUserID = "");
    #endif //!CLIENT_API_MODE
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

// IMPLEMENTATION

/* TODO
*  Refactoring SendRequest:
*  It is necessary to transfer method type
*  Refactoring GetPIn:  
*  
*/


bool ioteyeApi::SendRequest(uint8_t method, cpr::Payload &payload, const std::string& endpoint, cpr::Response* pResponse)
{
    // Формируем url для доступа к нужному эндпоинту
    std::string url = SERVER_URL;
    url += ':';
    url += SERVER_PORT;
   
    if(!endpoint.empty())
        url += endpoint;
    else
        url += "/";
    
    // Создаем переменную содержащую информацию об ответе
    cpr::Response r {};
    //выбор метода и отправка
    switch (method)
    {
    case POST:
        r = cpr::Post(cpr::Url{url}, payload);
        break;
    case GET:
        r = cpr::Get(cpr::Url{url}, payload);
        break;
    case PUT:
        r = cpr::Put(cpr::Url{url}, payload);
        break;
    case DELETE:
        r = cpr::Delete(cpr::Url{url}, payload);
        break;
    default:
        std::cerr << "Error: unknown method!" << std::endl;
    }
    
    // Если получен не 200, то выводим ошибку 
    if(r.status_code != cpr::status::HTTP_OK &&
        r.status_code != cpr::status::HTTP_CREATED)
    {
#ifdef DEBUG
        std::cerr   << "Error code: " << r.status_code << std::endl
                    << "Error cpr code: " << static_cast<uint16_t>(r.error.code) << std::endl 
                    << "Error: " << r.error.message << std::endl;
#endif //!DEBUG
        return true;
    }
    #ifdef DEBUG
    std::cout << "Response: " << r.text << std::endl;
    #endif //!DEBUG
    if(pResponse != nullptr)
        *pResponse = r;

    return false;
}

#ifdef CLIENT_API_MODE
std::string ioteyeApi::RegisterNewUser(const std::string& customUserID)
{
    cpr::Payload p{{"cmd", "ru"}};
    if(customUserID != "")
        p.Add({"customID", customUserID});

    std::string endpoint {ENDPOINT_USER};
    cpr::Response response{};
    if(!ioteyeApi::SendRequest(ioteyeApi::POST, p, endpoint, &response))
    {
        G_USERID = (GetValue(response.text, "userID"));
        s_ENDPOINT_USERID = '/' + G_USERID;
        #ifdef DEBUG
        std::cout << G_USERID << std::endl;
        #endif
        return G_USERID;
    }
    return "";
}
#endif //!CLIENT_API_MODE

bool ioteyeApi::GetDeviceStatus(const std::string& devName)
{
    cpr::Payload p{};
    p.Add({"userID", G_USERID});
    p.Add({"cmd", "ds"});

    auto device = s_devices.find(devName);
    if(device == s_devices.end())
    {
        std::cout << "Device with this name doesn`t exists!" << std::endl;
        return true;
    }
    std::string endpoint {ENDPOINT_USER};
    endpoint += s_ENDPOINT_USERID;
    endpoint += ENDPOINT_DEVICE;
    endpoint += '/';
    endpoint += std::to_string(device->second);
    endpoint += "/ds";
    cpr::Response response{};

    bool status = false;
    if(!ioteyeApi::SendRequest(ioteyeApi::GET, p, endpoint, &response))
    {
        status = (GetValue(response.text, "devStatus")[0] == '0') ? false : true;
        std::cout   << "Device with name: " << devName << " is " 
                    << ((status) ? "Online" : "Offline!") << std::endl;
        s_devicesStatus.at(devName) = std::stoi(GetValue(response.text, "devStatus"));
        
        return false;
    }
    std::cout << "Something went wrong!" << std::endl;
    return true;
}

bool ioteyeApi::RegisterNewDevice(const std::string& devName)
{
    auto device = s_devices.find(devName);
    if(device != s_devices.end())
    {
        std::cout << "Device with this name already exist!" << std::endl;
        return true;
    }
    cpr::Payload p{};
    p.Add({"userID", G_USERID});
    p.Add({"cmd", "rd"});

    std::string endpoint {ENDPOINT_USER};
    endpoint += s_ENDPOINT_USERID;
    endpoint += ENDPOINT_DEVICE;

    cpr::Response response{};
    if(!ioteyeApi::SendRequest(ioteyeApi::POST, p, endpoint, &response))
    {
        uint64_t devID = std::stoul(GetValue(response.text, "devID"));
        s_devices.emplace(devName, devID);
        s_devicesStatus.emplace(devName, false);
        std::cout   << "Device with name: " << devName << " added succesully! devID: " 
                    << devID << std::endl;
        return false;
    }
    return true;
}

bool ioteyeApi::CreateVirtualPin(const std::string &pinNumber, const std::string &dataType, const std::string& defaultData)
{
    cpr::Payload p{};
    p.Add({"userID", G_USERID});

    p.Add({"cmd", "cp"});
    p.Add({"pinNumber", pinNumber});
    p.Add({"dataType", dataType});
    p.Add({"value", defaultData});

    std::string endpoint {ENDPOINT_USER};
    endpoint += s_ENDPOINT_USERID;
    endpoint += ENDPOINT_PINS;
    #ifdef DEBUG
    std::cout << endpoint <<std::endl;
    #endif
    if(!ioteyeApi::SendRequest(ioteyeApi::POST, p, endpoint))
        return false;
    return true;
}

bool ioteyeApi::UpdateVirtualPin(const std::string &pinNumber, const std::string &value)
{
    cpr::Payload p{{"userID", G_USERID}};
    p.Add({"cmd", "up"});
    p.Add({"pinNumber", pinNumber});
    p.Add({"value", value});

    std::string endpoint {ENDPOINT_USER};
    endpoint += s_ENDPOINT_USERID;
    endpoint += ENDPOINT_PINS;
    
    if(!ioteyeApi::SendRequest(ioteyeApi::PUT, p, endpoint))
        return false;
    return true;
}

bool ioteyeApi::DeleteVirtualPin(const std::string &pinNumber)
{
    cpr::Payload p{{"userID", G_USERID}};
    p.Add({"cmd", "dp"});
    p.Add({"pinNumber", pinNumber});

    std::string endpoint {ENDPOINT_USER};
    endpoint += s_ENDPOINT_USERID;
    endpoint += ENDPOINT_PINS;

    if(!ioteyeApi::SendRequest(ioteyeApi::DELETE, p, endpoint))
        return false;
    return true;
}

std::string ioteyeApi::GetVirtualPin(const std::string &pinNumber)
{
    cpr::Response res {};
    cpr::Payload p{{"userID", G_USERID}};
    p.Add({"cmd", "pv"});
    p.Add({"pinNumber", pinNumber});

    std::string endpoint {ENDPOINT_USER};
    endpoint += s_ENDPOINT_USERID;
    endpoint += ENDPOINT_PINS;

    if (!ioteyeApi::SendRequest(ioteyeApi::POST, p, endpoint, &res))
        return GetValue(res.text);
    else
        return std::string("Error: " + res.text);
}

std::string ioteyeApi::GetValue(const std::string &responseText, const std::string& key)
{
    std::string value{};

    for (size_t i = responseText.find_last_of(key + '=') + 1; i < responseText.size(); i++)
        value += responseText.at(i);
    return value;
}


#endif //!IoTEYE_API_HPP

