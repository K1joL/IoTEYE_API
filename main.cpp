#include "Headers/IoTEYE_API.h"

int main()
{
    // Создаем пакет для передачи
    // Добавляем разные поля (ключ:значение)
    cpr::Payload p{{"userID", "someID"}};
    p.Add({"cmd", "cp"});
    p.Add({"authToken", "someToken"});
    p.Add({"pinNumber", "0"});
    p.Add({"dataType", "int"});
    p.Add({"defaultValue", "0"});
    // Передаем номер метода POST и сформированный пакет
    IoTEYE_API::SendRequest(IoTEYE_API::POST, p);
    return 0;
}