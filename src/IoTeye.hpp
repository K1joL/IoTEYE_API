#ifndef IOTEYE_HPP
#define IOTEYE_HPP

#include <Arduino.h>

#include <cassert>

#include "utility/ioteye_communication.hpp"
#include "utility/ioteye_config.hpp"
#include "utility/ioteye_debug.hpp"
#include "utility/ioteye_literals.hpp"
#include "utility/ioteye_payload.hpp"
#include "utility/ioteye_types.hpp"

// Commands
#define REGISTER_DEVICE "/rd"       // register_device
#define DELETE_DEVICE "/dd"         // delete_device
#define DEVICE_STATUS "/ds"         // device_status
#define DEVICE_STATUS_UPDATE "/us"  // device_status_update
#define CREATE_PIN "/cp"            // create_pin
#define UPDATE_PIN "/up"            // update_pin
#define DELETE_PIN "/dp"            // delete_pin
#define GET_PIN "/pv"               // get_pin

#define ENDPOINT_DEVICES "/devices"
#define ENDPOINT_PINS "/pins"

using ioteye::DebugLogger;
using ioteye::HttpCode;
using ioteye::HttpMethod;

class IoTeye {
public:
    IoTeye() {
    }
    IoTeye(IoTeye&& otherEye) = default;
    IoTeye& operator=(IoTeye&& otherEye) = default;
    ~IoTeye();

    void init(ioteye::IIoTeyeCommunication* commInterface) {
        m_commInterface = commInterface;
    }
    IoTeye& setToken(const String& token);
    IoTeye& setServerUrl(const String& url);
    IoTeye& setServerUrl(const String& host, int port);
    IoTeye& setLogger(DebugLogger* logger);
    IoTeye& setLogger(HardwareSerial* serial);
    IoTeye& setCommunicationInterface(ioteye::IIoTeyeCommunication *commInterface);
    void run(unsigned long updateInterval = 250);

    HttpCode createVirtualPin(const String& pinNumber, const String& dataType,
                              const String& defaultData = String());
    HttpCode createVirtualPin(const String& pinNumber, const String& dataType,
                              const int defaultData);
    HttpCode createVirtualPin(const String& pinNumber, const String& dataType,
                              const double defaultData);
    HttpCode writeVirtualPin(const String& pinNumber, const String& value);
    HttpCode deleteVirtualPin(const String& pinNumber);
    HttpCode updateDeviceStatus();

    // methods with return value
    uint16_t getDeviceStatus(const String& otherToken = String());
    String getVirtualPin(const String& pinNumber);
    // Use this methods with above for checks
    HttpCode getLastHttpCode();
    String getLastResponse();

private:
    ioteye::Response sendRequest(
        HttpMethod method, const String& endpoint,
        const ioteye::Payload& payload = ioteye::Payload());
    String extractValue(const String& responseText, const String& key);
    void log(const String& message);

private:
    ioteye::IIoTeyeCommunication* m_commInterface = nullptr;
    String m_token;
    String m_serverUrl;
    ioteye::Response m_lastResponse;
    // Stores the last time the device status was updated
    unsigned long m_lastUpdateTime = 0;
    ioteye::DebugLogger* m_logger = nullptr;
};
extern IoTeye iotEye;
#endif  // IOTEYE_HPP