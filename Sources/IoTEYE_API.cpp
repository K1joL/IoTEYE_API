#include "../Headers/IoTEYE_API.h"
#include "IoTEYE_API.h"

/* TODO
*  Refactoring SendRequest:
*  It is necessary to transfer method type
*
*/


bool ioteyeApi::SendRequest(uint8_t method, cpr::Payload &payload, cpr::Response* pResponse)
{
    // Формируем url для доступа к нужному эндпоинту
    std::string url = SERVER_URL;
    url += ':';
    url += SERVER_PORT;
    url += ENDPOINT_USER_PINS;
    // Создаем переменную содержащую информацию об ответе
    cpr::Response r {};
    //выбор метода и отправка
    switch (method)
    {
    case POST:
        r = cpr::Post(cpr::Url{url}, payload);
        break;
    case GET:
        r = cpr::Post(cpr::Url{url}, payload);
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
        std::cerr   << "Error code: " << r.status_code << std::endl
                    << "Error cpr code: " << static_cast<uint16_t>(r.error.code) << std::endl 
                    << "Error: " << r.error.message << std::endl;
        return true;
    }
    if(pResponse != nullptr)
        *pResponse = r;

    return false;
}

bool ioteyeApi::GetDeviceStatus()
{
    return false;
}

bool ioteyeApi::CreateVirtualPin(const std::string &pinNumber, const std::string &dataType, const std::string& defaultData)
{
    cpr::Payload p{{"userID", "someID"}};
    p.Add({"cmd", "cp"});
    //p.Add({"authToken", TOKEN});
    p.Add({"pinNumber", pinNumber});
    p.Add({"dataType", dataType});
    p.Add({"value", "0"});

    if(!ioteyeApi::SendRequest(ioteyeApi::POST, p))
        return false;
    return true;
}

bool ioteyeApi::UpdateVirtualPin(const std::string &pinNumber, std::string &value)
{
    cpr::Payload p{{"userID", "someID"}};
    p.Add({"cmd", "up"});
    //p.Add({"authToken", TOKEN});
    p.Add({"pinNumber", pinNumber});
    p.Add({"value", value});

    if(!ioteyeApi::SendRequest(ioteyeApi::PUT, p))
        return false;
    return true;
}

bool ioteyeApi::DeleteVirtualPin(const std::string &pinNumber)
{
    cpr::Payload p{{"userID", "someID"}};
    p.Add({"cmd", "dp"});
    //p.Add({"authToken", TOKEN});
    p.Add({"pinNumber", pinNumber});

    if(!ioteyeApi::SendRequest(ioteyeApi::DELETE, p))
        return false;
    return true;
}

std::string ioteyeApi::GetVirtualPin(const std::string &pinNumber)
{
    cpr::Response res {};
    cpr::Payload p{{"userID", "someID"}};
    p.Add({"cmd", "pv"});
    p.Add({"pinNumber", pinNumber});

    if (!ioteyeApi::SendRequest(ioteyeApi::GET, p, &res))
        return GetValue(res.text);
    else
        return res.text;
}

std::string ioteyeApi::GetValue(const std::string &responseText)
{
    std::string value{};
    
    for(size_t i = responseText.find_first_of('=')+1; i < responseText.size(); i++)
        value += responseText.at(i);
    return value;
}
