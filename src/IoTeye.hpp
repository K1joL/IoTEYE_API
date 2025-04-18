/*# MIT License

# Copyright (c) 2025 Shults Bogdan aka K1joL

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
*/

#ifndef IOTEYE_HPP
#define IOTEYE_HPP

#include <Arduino.h>

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