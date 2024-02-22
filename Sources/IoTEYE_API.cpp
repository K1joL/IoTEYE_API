#include "../Headers/IoTEYE_API.h"
// #include "IoTEYE_API.h"
using namespace IoTeyeDebug;

uint16_t IoTeye::sendRequest(uint8_t method,
                            const std::string &endpoint,
                            cpr::Payload &&payload,
                            cpr::Response *pResponse)
{
    // Формируем url для доступа к нужному эндпоинту
    std::string url = m_serverHost;
    url += ':';
    url += m_serverPort;

    if (!endpoint.empty())
        url += endpoint;
    else
        url += "/";

    // Создаем переменную содержащую информацию об ответе
    cpr::Response *r;

    if (pResponse != nullptr)
        r = pResponse;
    else
        r = new cpr::Response;

    // выбор метода и отправка
    switch (method)
    {
    case POST:
        *r = cpr::Post(cpr::Url{url}, payload);
        break;
    case GET:
        *r = cpr::Get(cpr::Url{url}, payload);
        break;
    case PUT:
        *r = cpr::Put(cpr::Url{url}, payload);
        break;
    case DELETE:
        *r = cpr::Delete(cpr::Url{url}, payload);
        break;
    default:
        debugMessage("Error: unknown method!");
        NEWLINE
        return 0;
        break;
    }

    // Если получен не 200, то выводим ошибку
    uint16_t code = r->status_code;
    if (code != cpr::status::HTTP_OK &&
        code != cpr::status::HTTP_CREATED)
    {
        debugMessage("Error code: ");
        debugMessage(code);
        NEWLINE
        debugMessage("Error cpr code: ");
        debugMessage(static_cast<uint16_t>(r->error.code));
        NEWLINE
        debugMessage("Error: ");
        debugMessage(r->error.message);
        NEWLINE
        debugMessage("Response: ");
        debugMessage(r->text);
        NEWLINE
        
        if (pResponse == nullptr)
            delete r;
        return code;
    }
    debugMessage("Response: ");
    debugMessage(r->text);
    NEWLINE

    if (pResponse == nullptr)
        delete r;

    return code;
}
uint16_t IoTeye::sendRequest(uint8_t method,
                            const std::string &endpoint,
                            cpr::Payload &payload,
                            cpr::Response *pResponse)
{
    // Формируем url для доступа к нужному эндпоинту
    std::string url = m_serverHost;
    url += ':';
    url += m_serverPort;

    if (!endpoint.empty())
        url += endpoint;
    else
        url += "/";

    // Создаем переменную содержащую информацию об ответе
    cpr::Response *r;

    if (pResponse != nullptr)
        r = pResponse;
    else
        r = new cpr::Response;

    // выбор метода и отправка
    switch (method)
    {
    case POST:
        *r = cpr::Post(cpr::Url{url}, payload);
        break;
    case GET:
        *r = cpr::Get(cpr::Url{url}, payload);
        break;
    case PUT:
        *r = cpr::Put(cpr::Url{url}, payload);
        break;
    case DELETE:
        *r = cpr::Delete(cpr::Url{url}, payload);
        break;
    default:
        debugMessage("Error: unknown method!");
        NEWLINE
        return 0;
        break;
    }

    // Если получен не 200, то выводим ошибку
    uint16_t code = r->status_code;
    if (code != cpr::status::HTTP_OK &&
        code != cpr::status::HTTP_CREATED)
    {
        debugMessage("Error code: ");
        debugMessage(code);
        NEWLINE
        debugMessage("Error cpr code: ");
        debugMessage(static_cast<uint16_t>(r->error.code));
        NEWLINE
        debugMessage("Error: ");
        debugMessage(r->error.message);
        NEWLINE
        debugMessage("Response: ");
        debugMessage(r->text);
        NEWLINE

        if (pResponse == nullptr)
            delete r;
        return code;
    }
    debugMessage("Response: ");
    debugMessage(r->text);
    NEWLINE

    if (pResponse == nullptr)
        delete r;

    return code;
}

std::string IoTeye::registerNewDevice()
{
    cpr::Payload p{{"cmd", "rd"}};
    std::string endpoint{m_endpointDevices};

    cpr::Response response{};
    std::string token{};
    if (!IoTeye::sendRequest(IoTeye::POST, endpoint, p, &response))
    {
        token = getValue(response.text, "token");
        debugMessage("Added succesully! token: ");
        debugMessage(token);
        NEWLINE
        return token;
    }
    return token;
}

uint16_t IoTeye::getDeviceStatus(const std::string &token)
{
    std::string endpoint{m_endpointDevices};
    endpoint += '/' + token;
    endpoint += "/ds";
    cpr::Response response{};

    bool status = 0;
    if (!IoTeye::sendRequest(IoTeye::GET, endpoint, cpr::Payload{}, &response))
    {
        status = (getValue(response.text, "devStatus")[0] + 48);

        debugMessage("Device is ");
        debugMessage(((status) ? "Online" : "Offline!"));
        NEWLINE
        return status;
    }
    debugMessage("Something went wrong!");
    NEWLINE

    return response.status_code;
}

uint16_t IoTeye::createVirtualPin(const std::string &token, const std::string &pinNumber, const std::string &dataType, const std::string &defaultData)
{
    cpr::Payload p{};
    p.Add({"token", token});
    p.Add({"cmd", "cp"});
    p.Add({"pinNumber", pinNumber});
    p.Add({"dataType", dataType});
    p.Add({"value", defaultData});

    std::string endpoint{m_endpointDevices};
    endpoint += '/' + token;
    endpoint += m_endpointPins;

    debugMessage(endpoint);
    NEWLINE
    
    return IoTeye::sendRequest(IoTeye::POST, endpoint, p);
}

uint16_t IoTeye::writeVirtualPin(const std::string &token, const std::string &pinNumber, const std::string &value)
{
    cpr::Payload p{{"token", token}};
    p.Add({"cmd", "up"});
    p.Add({"pinNumber", pinNumber});
    p.Add({"value", value});

    std::string endpoint{m_endpointDevices};
    endpoint += '/' + token;
    endpoint += m_endpointPins;

    return IoTeye::sendRequest(IoTeye::PUT, endpoint, p);
}

uint16_t IoTeye::deleteVirtualPin(const std::string &token, const std::string &pinNumber)
{
    cpr::Payload p{{"token", token}};
    p.Add({"cmd", "dp"});
    p.Add({"pinNumber", pinNumber});

    std::string endpoint{m_endpointDevices};
    endpoint += '/' + token;
    endpoint += m_endpointPins;

    return IoTeye::sendRequest(IoTeye::DELETE, endpoint, p);
}

std::string IoTeye::getVirtualPin(const std::string &token, const std::string &pinNumber)
{
    cpr::Response res{};
    cpr::Payload p{{"token", token}};
    p.Add({"cmd", "pv"});
    p.Add({"pinNumber", pinNumber});

    std::string endpoint{m_endpointDevices};
    endpoint += '/'+ token;
    endpoint += m_endpointPins;
    endpoint += '/' + pinNumber + "/pv";

    if (!IoTeye::sendRequest(IoTeye::GET, endpoint, p, &res))
        return getValue(res.text, "PinValue");
    else
    {
        debugMessage("Error: " + res.text);
        return std::string();
    }
}

std::string IoTeye::getValue(const std::string &responseText, const std::string &key)
{
    std::string value{};

    for (size_t i = responseText.find_last_of(key + '=') + 1; i < responseText.size(); i++)
        value += responseText.at(i);
    return value;
}

