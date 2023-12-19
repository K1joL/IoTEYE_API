#include "../Headers/IoTEYE_API.h"

/*
*   payload - request payload
*   
*   Returns:
*   false - OK
*   true - Error
*/
bool IoTEYE_API::SendRequest(const cpr::Payload &payload)
{
    std::string url = SERVER_URL;
    url += ':';
    url += SERVER_PORT;
    url += ENDPOINT;
    cpr::Response r = cpr::Post(cpr::Url{url},
                                payload);
    if(r.status_code != cpr::status::HTTP_OK)
    {
        std::cerr   << "Error code: " << static_cast<uint16_t>(r.error.code) << std::endl
                    << "Error: " << r.error.message << std::endl;
        return true;
    }
    return false;
}

bool IoTEYE_API::CreateVirtualPin(const std::string &name, const std::string &dataType)
{
    
    return false;
}

bool IoTEYE_API::GetDeviceStatus()
{
    return false;
}

bool IoTEYE_API::UpdateVirtualPin(const std::string &name, double data)
{
    return false;
}

bool IoTEYE_API::UpdateVirtualPin(const std::string &name, int data)
{
    return false;
}

bool IoTEYE_API::UpdateVirtualPin(const std::string &name, std::string &data)
{
    return false;
}
