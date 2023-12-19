#include "Headers/IoTEYE_API.h"

int main()
{
    cpr::Payload p{{"name", "V0"}};
    IoTEYE_API::SendRequestCurl(p);
    return 0;
}