#include "../Headers/IoTEYE_API.h"

using namespace IoTeyeDebug;

uint16_t IoTeye::sendRequest(uint8_t method,
                            const std::string &endpoint,
                            cpr::Payload &&payload)
{
    return sendRequest(method, endpoint, payload);
}
uint16_t IoTeye::sendRequest(uint8_t method,
                            const std::string &endpoint,
                            cpr::Payload &payload)
{
    // Формируем url для доступа к нужному эндпоинту
    std::string url = m_serverHost;
    url += ':';
    url += m_serverPort;

    if (!endpoint.empty())
        url += endpoint;
    else
        url += "/";

    // выбор метода и отправка
    switch (method)
    {
    case POST:
        m_lastResponse = cpr::Post(cpr::Url{url}, payload);
        break;
    case GET:
        m_lastResponse = cpr::Get(cpr::Url{url}, payload);
        break;
    case PUT:
        m_lastResponse = cpr::Put(cpr::Url{url}, payload);
        break;
    case DELETE:
        m_lastResponse = cpr::Delete(cpr::Url{url}, payload);
        break;
    default:
        debugMessageln("Error: unknown method!");
        return 0;
    }

    uint16_t code = m_lastResponse.status_code;
    if (code >= 200 && code < 300)
    {
        debugMessage("Error code: ");
        debugMessageln(code);
        debugMessage("Error cpr code: ");
        debugMessageln(static_cast<uint16_t>(m_lastResponse.error.code));
        debugMessage("Error: ");
        debugMessageln(m_lastResponse.error.message);
        debugMessage("Response: ");
        debugMessageln(m_lastResponse.text);
        return code;
    }
    debugMessageln("Response: " + m_lastResponse.text);

    return code;
}

std::string IoTeye::registerNewDevice()
{
    cpr::Payload p{{"cmd", "rd"}};
    std::string endpoint{m_endpointDevices};

    std::string token{};
    if (IoTeye::sendRequest(IoTeye::POST, endpoint, p) == cpr::status::HTTP_CREATED)
    {
        token = getValue(m_lastResponse.text, "token");
        debugMessageln("Added succesully! token: " + token);
        return token;
    }
    return token;
}

uint16_t IoTeye::getDeviceStatus(const std::string &token)
{
    std::string endpoint{m_endpointDevices};
    endpoint += '/' + token;
    endpoint += "/ds";

    bool status = 0;
    if (IoTeye::sendRequest(IoTeye::GET, endpoint, cpr::Payload{}) == cpr::status::HTTP_OK)
    {
        status = (getValue(m_lastResponse.text, "devStatus")[0] + 48);

        debugMessage("Device is ");
        debugMessageln(((status) ? "Online" : "Offline!"));
        return status;
    }

    return m_lastResponse.status_code;
}

uint16_t IoTeye::deleteDevice(const std::string &token)
{
    std::string endpoint{m_endpointDevices};
    endpoint += '/' + token;
    endpoint += "/dd";

    return IoTeye::sendRequest(IoTeye::DELETE, endpoint);
}
uint16_t IoTeye::createVirtualPin(const std::string &token, const std::string &pinNumber, const std::string &dataType, const std::string &defaultData)
{
    cpr::Payload p{};
    p.Add({"token", token});
    p.Add({"cmd", "cp"});
    p.Add({"pinNumber", pinNumber});
    p.Add({"dataType", dataType});
    p.Add({"defValue", defaultData});

    std::string endpoint{m_endpointDevices};
    endpoint += m_endpointPins;

    return IoTeye::sendRequest(IoTeye::POST, endpoint, p);
}

uint16_t IoTeye::writeVirtualPin(const std::string &token, const std::string &pinNumber, const std::string &value)
{
    cpr::Payload p{{"token", token}};
    p.Add({"cmd", "up"});
    p.Add({"pinNumber", pinNumber});
    p.Add({"value", value});

    std::string endpoint{m_endpointDevices};
    endpoint += m_endpointPins;

    return IoTeye::sendRequest(IoTeye::PUT, endpoint, p);
}

uint16_t IoTeye::deleteVirtualPin(const std::string &token, const std::string &pinNumber)
{
    std::string endpoint{m_endpointDevices};
    endpoint += '/' + token;
    endpoint += m_endpointPins;
    endpoint += '/' + pinNumber;
    endpoint += "/dp";

    return IoTeye::sendRequest(IoTeye::DELETE, endpoint);
}

std::string IoTeye::getVirtualPin(const std::string &token, const std::string &pinNumber)
{
    std::string endpoint{m_endpointDevices};
    endpoint += '/'+ token;
    endpoint += m_endpointPins;
    endpoint += '/' + pinNumber;
    endpoint += "/pv";

    if (IoTeye::sendRequest(IoTeye::GET, endpoint, cpr::Payload{}) == cpr::status::HTTP_OK)
        return getValue(m_lastResponse.text, "PinValue");
    return std::string("VPGetError");
}

std::string IoTeye::getValue(const std::string &responseText, const std::string &key)
{
    std::string value{responseText.substr(responseText.find_first_of(key + '=') + key.length() + 1, responseText.length())};
    return value;
}

