const char* USERID = "";
#include "Headers/IoTEYE_API.h"
#include <iostream>
#include <thread>
using namespace std;

int main()
{
    IoTeye eye;
    // eye.setServerHost("10.10.90.32");
    std::string token{};
    token = eye.registerNewDevice();
    eye.createVirtualPin(token, "1", "int", "0");
    eye.getVirtualPin(token, "1");
    eye.writeVirtualPin(token, "1", "1337");
    eye.getVirtualPin(token, "1");
    eye.deleteVirtualPin(token, "1");
    eye.getDeviceStatus(token);
    eye.deleteDevice(token);
    // cpr::Payload p{{"cmd", "cp"}};
    // p.Add({"token", token});
    // p.Add({"pinNumber", "1"});
    // p.Add({"dataType", "int"});
    // p.Add({"value", "0"});
    // cpr::Response r2;
    // // eye.sendRequest(IoTeye::POST, "/devices", p ,&r2);
    // eye.sendRequest(IoTeye::POST, "/devices/pins", p ,&r2);

    return 0;
}