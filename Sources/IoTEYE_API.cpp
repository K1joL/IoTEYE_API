#include "../Headers/IoTEYE_API.h"
// #include "IoTEYE_API.h"

bool ioteyeApi::sendRequest(uint8_t method,
                            const std::string &endpoint,
                            cpr::Payload &&payload,
                            cpr::Response *pResponse)
{
    // Формируем url для доступа к нужному эндпоинту
    std::string url = s_SERVER_URL;
    url += ':';
    url += s_SERVER_PORT;

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
        return true;
        break;
    }

    // Если получен не 200, то выводим ошибку
    if (r->status_code != cpr::status::HTTP_OK &&
        r->status_code != cpr::status::HTTP_CREATED)
    {
        debugMessage("Error code: ");
        debugMessage(r->status_code);
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
        return true;
    }
    debugMessage("Response: ");
    debugMessage(r->text);
    NEWLINE

    if (pResponse == nullptr)
        delete r;

    return false;
}
bool ioteyeApi::sendRequest(uint8_t method,
                            const std::string &endpoint,
                            cpr::Payload &payload,
                            cpr::Response *pResponse)
{
    // Формируем url для доступа к нужному эндпоинту
    std::string url = s_SERVER_URL;
    url += ':';
    url += s_SERVER_PORT;

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
        return true;
        break;
    }

    // Если получен не 200, то выводим ошибку
    if (r->status_code != cpr::status::HTTP_OK &&
        r->status_code != cpr::status::HTTP_CREATED)
    {
        debugMessage("Error code: ");
        debugMessage(r->status_code);
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
        return true;
    }
    debugMessage("Response: ");
    debugMessage(r->text);
    NEWLINE

    if (pResponse == nullptr)
        delete r;

    return false;
}

std::string ioteyeApi::registerNewUser(const std::string &customUserID)
{
    cpr::Payload p{{"cmd", "ru"}};
    if (customUserID != "")
        p.Add({"customID", customUserID});

    std::string endpoint{s_ENDPOINT_USERS};
    cpr::Response response{};
    if (!ioteyeApi::sendRequest(ioteyeApi::POST, endpoint, p, &response))
    {
        G_USERID = (getValue(response.text, "userID"));
        s_ENDPOINT_USERID = '/' + G_USERID;

        debugMessage(G_USERID);
        NEWLINE
        return G_USERID;
    }
    return "";
}

bool ioteyeApi::getDeviceStatus(const std::string &devName)
{
    cpr::Payload p{};
    p.Add({"userID", G_USERID});
    p.Add({"cmd", "ds"});

    auto device = s_devices.find(devName);
    if (device == s_devices.end())
    {
        debugMessage("Device with this name doesn`t exists!");
        NEWLINE
        return true;
    }
    std::string endpoint{s_ENDPOINT_USERS};
    endpoint += s_ENDPOINT_USERID;
    endpoint += s_ENDPOINT_DEVICES;
    endpoint += '/' + std::to_string(device->second) + "/ds";
    cpr::Response response{};

    bool status = false;
    if (!ioteyeApi::sendRequest(ioteyeApi::GET, endpoint, p, &response))
    {
        status = (getValue(response.text, "devStatus")[0] == '0') ? false : true;

        debugMessage("Device with name: ");
        debugMessage(devName);
        debugMessage(" is ");
        debugMessage(((status) ? "Online" : "Offline!"));
        NEWLINE

        s_devicesStatus.at(devName) = std::stoi(getValue(response.text, "devStatus"));

        return false;
    }
    debugMessage("Something went wrong!");
    NEWLINE

    return true;
}

bool ioteyeApi::registerNewDevice(const std::string &devName)
{
    auto device = s_devices.find(devName);
    if (device != s_devices.end())
    {
        debugMessage("Device with this name already exist!");
        NEWLINE

        return true;
    }
    cpr::Payload p{};
    p.Add({"userID", G_USERID});
    p.Add({"cmd", "rd"});

    std::string endpoint{s_ENDPOINT_USERS};
    endpoint += s_ENDPOINT_USERID;
    endpoint += s_ENDPOINT_DEVICES;

    cpr::Response response{};
    if (!ioteyeApi::sendRequest(ioteyeApi::POST, endpoint, p, &response))
    {
        uint64_t devID = std::stoul(getValue(response.text, "devID"));
        s_devices.emplace(devName, devID);
        s_devicesStatus.emplace(devName, false);

        debugMessage("Device with name: ");
        debugMessage(devName);
        debugMessage(" added succesully! devID: ");
        debugMessage(devID);
        NEWLINE

        return false;
    }
    return true;
}

bool ioteyeApi::createVirtualPin(const std::string &pinNumber, const std::string &dataType, const std::string &defaultData)
{
    cpr::Payload p{};
    p.Add({"userID", G_USERID});

    p.Add({"cmd", "cp"});
    p.Add({"pinNumber", pinNumber});
    p.Add({"dataType", dataType});
    p.Add({"value", defaultData});

    std::string endpoint{s_ENDPOINT_USERS};
    endpoint += s_ENDPOINT_USERID;
    endpoint += s_ENDPOINT_PINS;

    debugMessage(endpoint);
    NEWLINE

    if (!ioteyeApi::sendRequest(ioteyeApi::POST, endpoint, p))
        return false;
    return true;
}

bool ioteyeApi::updateVirtualPin(const std::string &pinNumber, const std::string &value)
{
    cpr::Payload p{{"userID", G_USERID}};
    p.Add({"cmd", "up"});
    p.Add({"pinNumber", pinNumber});
    p.Add({"value", value});

    std::string endpoint{s_ENDPOINT_USERS};
    endpoint += s_ENDPOINT_USERID;
    endpoint += s_ENDPOINT_PINS;

    if (!ioteyeApi::sendRequest(ioteyeApi::PUT, endpoint, p))
        return false;
    return true;
}

bool ioteyeApi::deleteVirtualPin(const std::string &pinNumber)
{
    cpr::Payload p{{"userID", G_USERID}};
    p.Add({"cmd", "dp"});
    p.Add({"pinNumber", pinNumber});

    std::string endpoint{s_ENDPOINT_USERS};
    endpoint += s_ENDPOINT_USERID;
    endpoint += s_ENDPOINT_PINS;

    if (!ioteyeApi::sendRequest(ioteyeApi::DELETE, endpoint, p))
        return false;
    return true;
}

std::string ioteyeApi::getVirtualPin(const std::string &pinNumber)
{
    cpr::Response res{};
    cpr::Payload p{{"userID", G_USERID}};
    p.Add({"cmd", "pv"});
    p.Add({"pinNumber", pinNumber});

    std::string endpoint{s_ENDPOINT_USERS};
    endpoint += s_ENDPOINT_USERID;
    endpoint += s_ENDPOINT_PINS;
    endpoint += '/' + pinNumber + "/pv";

    if (!ioteyeApi::sendRequest(ioteyeApi::GET, endpoint, p, &res))
        return getValue(res.text, "PinValue");
    else
    {
        debugMessage("Error: " + res.text);
        return std::string();
    }
}

std::string ioteyeApi::getValue(const std::string &responseText, const std::string &key)
{
    std::string value{};

    for (size_t i = responseText.find_last_of(key + '=') + 1; i < responseText.size(); i++)
        value += responseText.at(i);
    return value;
}

void ioteyeApi::setServerUrl(const std::string &serverUrl)
{
    s_SERVER_URL = serverUrl;
}

void ioteyeApi::setServerPort(const std::string &serverPort)
{
    s_SERVER_PORT = serverPort;
}

void setUsersEndpoint(const std::string &usersEndpoint)
{
    s_ENDPOINT_USERS = usersEndpoint;
}

void setPinsEndpoint(const std::string &pinsEndpoint)
{
    s_ENDPOINT_PINS = pinsEndpoint;
}

void setDeviceEndpoint(const std::string &deviceEndpoint)
{
    s_ENDPOINT_DEVICES = deviceEndpoint;
}
