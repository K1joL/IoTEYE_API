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

#include <cpr/cpr.h>
#include <iostream>
#include <string>

namespace IoTEYE_API
{
    bool SendRequest(uint8_t method, const cpr::Payload &payload);
    bool CreateVirtualPin(const std::string& name, const std::string& dataType);
    bool GetDeviceStatus();
    bool UpdateVirtualPin(const std::string& name, double data);
    bool UpdateVirtualPin(const std::string& name, int data);
    bool UpdateVirtualPin(const std::string& name, std::string& data);

    enum HTTP_METHOD
    {
        GET,
        POST,
        PUT,
        DELETE
    };
}

#endif //IoTEYE_API_H

