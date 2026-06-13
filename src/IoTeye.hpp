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
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "utility/ioteye_communication.hpp"
#include "utility/ioteye_debug.hpp"
#include "utility/ioteye_endpoint_manager.hpp"
#include "utility/ioteye_literals.hpp"
#include "utility/ioteye_payload.hpp"
#include "utility/ioteye_types.hpp"

using ioteye::DebugLogger;
using ioteye::HttpCode;
using ioteye::HttpMethod;
using ioteye::LogLevel;

class IoTeye {
public:
    IoTeye();
    IoTeye(IoTeye&& otherEye) = default;
    IoTeye& operator=(IoTeye&& otherEye) = default;
    ~IoTeye();

    void init();
    IoTeye& setToken(const char* token);
    IoTeye& setServerUrl(const char* url);
    IoTeye& setServerUrl(const char* host, int port);
    IoTeye& setLogger(const DebugLogger& logger);
    IoTeye& setSerial(HardwareSerial* serial);
    IoTeye& setCommunicationInterface(
        ioteye::IIoTeyeCommunication* commInterface);
    void run(unsigned long updateInterval = 250);

    HttpCode createVirtualPin(const char* pinNumber, const char* dataType,
                              const char* defaultData = "");
    HttpCode createVirtualPin(const char* pinNumber, const char* dataType,
                              const int defaultData);
    HttpCode createVirtualPin(const char* pinNumber, const char* dataType,
                              const double defaultData, uint8_t precision = 5);
    HttpCode writeVirtualPin(const char* pinNumber, const char* value);
    HttpCode writeVirtualPin(const char* pinNumber, const int value);
    HttpCode writeVirtualPin(const char* pinNumber, const double value,
                             uint8_t precision = 5);
    HttpCode deleteVirtualPin(const char* pinNumber);
    HttpCode updateDeviceStatus();
    HttpCode registerDevice();
    HttpCode deleteDevice(const char* token);
    HttpCode deleteDevice();
    HttpCode getEndpoints();

    // methods with return value
    uint16_t getDeviceStatus(const char* otherToken = "");
    const char* getVirtualPin(const char* pinNumber, const char* otherToken = "");
    // Use this methods with above for checks
    HttpCode getLastHttpCode();
    const char* getLastResponse();

public:
    enum STATES { ONLINE, OFFLINE, OUTDATED, MAX_STATES };

private:
    ioteye::Response sendRequest(
        HttpMethod method, const char* endpoint,
        const ioteye::Payload& payload = ioteye::Payload());
    const char* extractValue(const char* responseText, const char* key);
    void buildUrl(char* url, const char* endpoint);

private:
    ioteye::IIoTeyeCommunication* m_commInterface = nullptr;
    char m_token[MAX_TOKEN_LENGTH] = {0};
    char m_serverUrl[MAX_URL_LENGTH] = {0};
    ioteye::Response m_lastResponse;
    ioteye::EndpointManager m_epManager;
    // Stores the last time the device status was updated
    unsigned long m_lastUpdateTime = 0;
    ioteye::DebugLogger m_logger;

    bool m_preInit = true;
    bool m_isReady = false;
    bool m_selfRegistered = false;
};
extern IoTeye iotEye;
#endif  // IOTEYE_HPP