const char* USERID = "";
#include "Headers/IoTEYE_API.h"

#include <thread>

int main()
{
    // Передаем номер метода POST и сформированный пакет
    // USERID = ioteyeApi::RegisterNewUser("kijol").c_str();
    std::cout << ioteyeApi::RegisterNewUser("kijol") << std::endl;
    // ioteyeApi::CreateVirtualPin("0", "int", "199");
    // std::cout << ioteyeApi::GetVirtualPin("0") << std::endl;
    // std::cout << ioteyeApi::UpdateVirtualPin("0","1") << std::endl;
    // std::cout << ioteyeApi::GetVirtualPin("0") << std::endl;
    // std::cout << ioteyeApi::DeleteVirtualPin("0") << std::endl;
    // std::cout << ioteyeApi::GetVirtualPin("0") << std::endl;
    ioteyeApi::RegisterNewDevice("device1");
    ioteyeApi::GetDeviceStatus("device1");
    cpr::Payload p{{"cmd","us"}};
    char key;
    while(true)
    {
        
        std::this_thread::sleep_for(std::chrono::milliseconds(4));
        ioteyeApi::SendRequest(ioteyeApi::GET, p, "/user/kijol/devices/1/us");
    }
    return 0;
}