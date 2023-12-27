#define CLIENT_API_MODE
#define DEBUG
const char* USERID = "";

#include "Sources/IoTEYE_API.hpp"

int main()
{
    // Передаем номер метода POST и сформированный пакет
    USERID = ioteyeApi::RegisterNewUser("Kijol").c_str();
    std::cout << USERID << std::endl;
    ioteyeApi::CreateVirtualPin("0", "int", "199");
    std::cout << ioteyeApi::GetVirtualPin("0") << std::endl;
    std::cout << ioteyeApi::UpdateVirtualPin("0","1") << std::endl;
    std::cout << ioteyeApi::GetVirtualPin("0") << std::endl;
    std::cout << ioteyeApi::DeleteVirtualPin("0") << std::endl;
    std::cout << ioteyeApi::GetVirtualPin("0") << std::endl;

    // cpr::Payload p{{"userID", "kukaID"}};
    // p.Add({"cmd", "pv"});
    // p.Add({"pinNumber", "0"});
    // p.Add({"dataType", "int"});
    // p.Add({"value", "0"});
    return 0;
}