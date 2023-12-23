#include "Headers/IoTEYE_API.h"

int main()
{
    cpr::Payload p{{"userID", "someID"}};
    p.Add({"authToken", "someToken"});
    p.Add({"pinNumber", "0"});
    p.Add({"dataType", "int"});
    p.Add({"value", "0"});
    IoTEYE_API::SendRequest(IoTEYE_API::POST, p);
    return 0;
}