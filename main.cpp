const char* USERID = "";
#include "Headers/IoTEYE_API.h"
#include <iostream>
#include <thread>
using namespace std;

int main()
{
    IoTeye eye;
    eye.setServerHost("10.10.90.32");
    // std::string token{};
    // token = eye.registerNewDevice();
    // eye.createVirtualPin(token, "1", "int", "0");
    // std::cout << eye.getVirtualPin(token, "1") << std::endl;
    cpr::Payload p{{"cmd", "rd"}};
    cpr::Response r2;
    eye.sendRequest(IoTeye::POST, "/devices", p ,&r2);
    eye.sendRequest(IoTeye::GET, "/devices/eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJkZXZpY2VJRCI6IjEifQ.3O3RIiMFsE2GwmA4FqpHU9rBSVkKDpjKpFta22yxSy4/ds", p ,&r2);
        
    return 0;
}