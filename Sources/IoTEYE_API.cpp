#include "../Headers/IoTEYE_API.h"
#include "IoTEYE_API.h"

/* TODO
*  Refactoring SendRequest:
*  It is necessary to transfer method type
*
*/

/*
*   payload - request payload
*   pResponse - pointer to cpr::Response if you want to get response
*
*   Returns:
*   false - OK
*   true - Error
*/
bool IoTEYE_API::SendRequest(uint8_t method, const cpr::Payload &payload, cpr::Response* pResponse)
{
    // Формируем url для доступа к нужному эндпоинту
    std::string url = SERVER_URL;
    url += ':';
    url += SERVER_PORT;
    url += ENDPOINT;
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
    if(r.status_code != cpr::status::HTTP_OK || 
        r.status_code != cpr::status::HTTP_CREATED)
    {
        std::cerr   << "Error code: " << static_cast<uint16_t>(r.error.code) << std::endl
                    << "Error: " << r.error.message << std::endl;
        return true;
    }   
    if(pResponse != nullptr)
        *pResponse = r;

    return false;
}

bool IoTEYE_API::CreateVirtualPin(const std::string &pinNumber, const std::string &dataType)
{
    
    return false;
}

bool IoTEYE_API::GetDeviceStatus()
{
    return false;
}

bool IoTEYE_API::UpdateVirtualPin(const std::string &pinNumber, double data)
{
    return false;
}

bool IoTEYE_API::UpdateVirtualPin(const std::string &pinNumber, int data)
{
    return false;
}

bool IoTEYE_API::UpdateVirtualPin(const std::string &pinNumber, std::string &data)
{
    return false;
}
bool IoTEYE_API::DeleteVirtualPin(const std::string &pinNumber)
{
    return false;
}

std::string IoTEYE_API::GetVirtualPin(const std::string &pinNumber)
{
    
    return std::string{};
}
