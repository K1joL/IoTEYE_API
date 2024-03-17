#include "IoTEYE_API.h"
#include <iostream>

using namespace std;

bool isPinNumberValid(std::string &number)
{
    int pinNumber = stoi(number);
    return (pinNumber < 256 && pinNumber > 0);
}
bool isPinTypeValid(std::string &type)
{
    return type == VPSTRING || type == VPINT || type == VPDOUBLE;
}

int main()
{
    std::cout << "Enter the server`s host: ";
    std::string host;
    std::cin >> host;

    std::cout << "Enter the server`s port: ";
    std::string port;
    std::cin >> port;

    // Create a new instance of the IoTeye class
    IoTeye ioteye(host, port);

    char option, prevOption;
    uint16_t codeResult = 0;
    std::string pin{},
                pinType{},
                pinValue{},
                defaultData{},
                value{},
                temp{};
    bool exit = false;
    while (!exit)
    {
        std::cout << "\nPlease select one of the following options:\n";
        std::cout << "1. Register new device\n";
        std::cout << "2. Change current token\n";
        std::cout << "3. Show current token\n";
        std::cout << "4. Get current status of device\n";
        std::cout << "5. Create virtual pin\n";
        std::cout << "6. Write value to virtual pin\n";
        std::cout << "7. Get current value of virtual pin\n";
        std::cout << "8. Delete virtual pin\n";
        std::cout << "9. Delete device\n";
        std::cout << "q. Exit\n";
        prevOption = option;
        std::cin >> option;
        static std::string token{};
        switch (option)
        {
        case '1':
            // Register a new device with the IoTeye API
            token = ioteye.registerNewDevice();
            if (token == "")
                std::cout << "Error registering new device" << std::endl;
            else
                std::cout << "Registered new device with token: " << token << std::endl;
            break;
        case '2':
            std::cout << "Enter the token: ";
            std::cin >> token;
        case '3':
            std::cout << "Current token is " << token;
            break;
        case '4':
            // Get the status of a device on the IoTeye API
            codeResult = ioteye.getDeviceStatus(token);
            if (codeResult < 3)
                switch (codeResult)
                {
                case 0:
                    std::cout << "Device is online" << std::endl;
                    break;
                case 1:
                    std::cout << "Device is offline" << std::endl;
                    break;
                case 2:
                    std::cout << "Device data is out of date" << std::endl;
                    break;
                default:
                    break;
                }
            else
                std::cout << "Error getting device status" << std::endl;

            break;
        case '5':
            // Create a virtual pin on the IoTeye API
            pin.clear();
            pinType.clear();
            defaultData.clear();

            std::cout << "Enter the number of pin you want to create: ";
            do
            {
                if (!pin.empty())
                    std::cout << "Please try again! The number must be greater than 0 and less than 256!";
                std::cin >> pin;
            } while (!isPinNumberValid(pin));

            std::cout << "Enter the type of pin : \"string\", \"int\", \"double\"" << std::endl
                      << "Type: ";
            do
            {
                if(!pinType.empty())
                    std::cout << "Please try again! The type must be \"string\", \"int\", \"double\"!";
                std::cin >> pinType;
            } while (!isPinTypeValid(pinType));

            std::cout << "Enter the default data that pin will contain." << std::endl
                      << "Leave empty if dont want to write default data." << std::endl
                      << "Default data: ";
            std::cin >> defaultData;

            codeResult = ioteye.createVirtualPin(token, pin, pinType, defaultData);
            if (codeResult == 201)
                std::cout << "Created virtual pin with ID: " << pin << std::endl;
            else
                std::cout << "Error creating virtual pin" << std::endl;

            break;
        case '6':
            // Write to a virtual pin on the IoTeye API
            std::cout << "Enter the number of pin you want to write to." << std::endl;
            if(prevOption == '5' || prevOption == '6' || prevOption == '7')
                std::cout << "Leave empty if you want to use number of pin used before.";
            std::cout << "Pin number: ";
            cin >> temp;
            if(!temp.empty())
            {
                pin = temp;
                while (!isPinNumberValid(pin))
                {
                    std::cout << "Please try again! The number must be greater than 0 and less than 256!";
                    std::cin >> pin;
                }
            }
                
            std::cout << "Enter the value you want to write: ";
            std::cin >> value;

            codeResult = ioteye.writeVirtualPin(token, pin, value);
            if (codeResult == 200)
                std::cout << "Value \"" << value << "\" wrote to virtual pin with ID: " << pin << std::endl;
            else
                std::cout << "Error writing to virtual pin" << std::endl;

            break;
        case '7':
            // Get the value of a virtual pin from the IoTeye API

            std::cout << "Enter the number of pin you want to get." << std::endl;
            if(prevOption == '5' || prevOption == '6' || prevOption == '7')
                std::cout << "Leave empty if you want to use number of pin used before.";
            std::cout << "Pin number: ";
            cin >> temp;
            if(!temp.empty())
            {
                pin = temp;
                while (!isPinNumberValid(pin))
                {
                    std::cout << "Please try again! The number must be greater than 0 and less than 256!";
                    std::cin >> pin;
                }
            }

            pinValue = ioteye.getVirtualPin(token, pin);
            if (pinValue != "VPGetError")
                std::cout << "Got value of virtual pin: " << pinValue << std::endl;
            else
                std::cout << "Error getting value of virtual pin" << std::endl;

            break;
        case '8':
            // Delete a virtual pin on the IoTeye API

            std::cout << "Enter the number of pin you want to delete." << std::endl;
            if(prevOption == '5' || prevOption == '6' || prevOption == '7')
                std::cout << "Leave empty if you want to use number of pin used before.";
            std::cout << "Pin number: ";
            cin >> temp;
            if(!temp.empty())
            {
                pin = temp;
                while (!isPinNumberValid(pin))
                {
                    std::cout << "Please try again! The number must be greater than 0 and less than 256!";
                    std::cin >> pin;
                }
            }

            codeResult = ioteye.deleteVirtualPin(token, pin);
            if (codeResult == 200)
                std::cout << "Deleted virtual pin with ID: " << token << std::endl;
            else
                std::cout << "Error deleting virtual pin" << std::endl;

            break;
        case '9':
            // Delete a virtual device on the IoTeye API
            std::cout << "Are you sure that you want to delete device?" << std::endl
                      << "All of the pins will be deleted too." << std::endl
                      << "(Y)es or (N)o: ";
            cin >> temp;
            if(temp == "No" || temp[0] == 'N' || temp[0] == 'n')
                break;
            codeResult = ioteye.deleteDevice(token);
            if (codeResult == 200)
                std::cout << "Deleted device with ID: " << token << std::endl;
            else
                std::cout << "Error deleting device" << std::endl;
            break;
        case 'q':
            exit = true;
        }
    }

    return 0;
}