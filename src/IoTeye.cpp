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

#include "IoTeye.hpp"

IoTeye::~IoTeye() {
    if (m_logger != nullptr)
        delete m_logger;
}

IoTeye &IoTeye::setToken(const String &token) {
    log("Setting token: " + token);
    m_token = token;
    return *this;
}
IoTeye &IoTeye::setServerUrl(const String &url) {
    log("Setting server URL: " + url);
    m_serverUrl = url;
    return *this;
}

IoTeye &IoTeye::setServerUrl(const String &host, int port) {
    log("Setting server URL: " + host + ":" + String(port));
    m_serverUrl = host + String(port);
    return *this;
}

IoTeye &IoTeye::setLogger(DebugLogger *logger) {
    m_logger = logger;
    return *this;
}

IoTeye &IoTeye::setLogger(HardwareSerial *serial) {
    DebugLogger *logger = new DebugLogger(serial);
    m_logger = logger;
    return *this;
}

IoTeye &IoTeye::setCommunicationInterface(
    ioteye::IIoTeyeCommunication *commInterface) {
    m_commInterface = commInterface;
    return *this;
}

void IoTeye::run(unsigned long updateInterval) {
    unsigned long currentTime = millis();

    // Check if it's time to update the device status
    if (currentTime - m_lastUpdateTime >= updateInterval) {
        log("Updating device status");
        updateDeviceStatus();
        m_lastUpdateTime = currentTime;  // Reset the timer
    }
}

HttpCode IoTeye::createVirtualPin(const String &pinNumber,
                                  const String &dataType,
                                  const String &defaultData) {
    String endpoint;
    endpoint.reserve(256);  // Pre-allocate a reasonable buffer
    endpoint = ENDPOINT_DEVICES;
    endpoint += '/' + m_token;
    endpoint += ENDPOINT_PINS;
    endpoint += '/' + pinNumber;
    endpoint += '/' + dataType;
    endpoint += '/' + defaultData;
    endpoint += CREATE_PIN;

    m_logger->log(DebugLogger::INFO,
                  "Creating virtual pin with endpoint: " + endpoint);
    return sendRequest(HttpMethod::POST, endpoint).statusCode;
}

HttpCode IoTeye::createVirtualPin(const String &pinNumber,
                                  const String &dataType, int defaultData) {
    m_logger->log(
        DebugLogger::INFO,
        "Creating virtual pin with default data (int): " + String(defaultData));
    return createVirtualPin(pinNumber, dataType, String(defaultData));
}

HttpCode IoTeye::createVirtualPin(const String &pinNumber,
                                  const String &dataType, double defaultData) {
    m_logger->log(DebugLogger::INFO,
                  "Creating virtual pin with default data (double): " +
                      String(defaultData));
    return createVirtualPin(pinNumber, dataType, String(defaultData));
}

HttpCode IoTeye::writeVirtualPin(const String &pinNumber, const String &value) {
    String endpoint;
    endpoint = ENDPOINT_DEVICES;
    endpoint += '/' + m_token;
    endpoint += ENDPOINT_PINS;
    endpoint += '/' + pinNumber;
    endpoint += '/' + value;
    endpoint += UPDATE_PIN;

    m_logger->log(DebugLogger::INFO,
                  "Writing virtual pin with endpoint: " + endpoint);
    return sendRequest(HttpMethod::PUT, endpoint).statusCode;
}

HttpCode IoTeye::deleteVirtualPin(const String &pinNumber) {
    String endpoint;
    endpoint += ENDPOINT_DEVICES;
    endpoint += "/" + m_token;
    endpoint += ENDPOINT_PINS;
    endpoint += "/" + pinNumber;
    endpoint += DELETE_PIN;

    m_logger->log(DebugLogger::INFO,
                  "Deleting virtual pin with endpoint: " + endpoint);
    return sendRequest(HttpMethod::DELETE, endpoint).statusCode;
}

String IoTeye::getVirtualPin(const String &pinNumber) {
    String endpoint;
    endpoint = ENDPOINT_DEVICES;
    endpoint += '/' + m_token;
    endpoint += ENDPOINT_PINS;
    endpoint += '/' + pinNumber;
    endpoint += GET_PIN;

    m_logger->log(DebugLogger::INFO,
                  "Getting virtual pin with endpoint: " + endpoint);
    if (sendRequest(HttpMethod::GET, endpoint).statusCode.isSuccess())
        return extractValue(m_lastResponse.body, "PinValue");
    return String();
}

uint16_t IoTeye::getDeviceStatus(const String &otherToken) {
    String token{};
    if (otherToken.isEmpty())
        return 0;
    else
        token = otherToken;

    String endpoint;
    endpoint += ENDPOINT_DEVICES;
    endpoint += "/" + m_token + DEVICE_STATUS;

    m_logger->log(DebugLogger::INFO,
                  "Getting device status with endpoint: " + endpoint);
    if (sendRequest(HttpMethod::GET, endpoint).statusCode.isSuccess())
        return 0;
    if (!m_lastResponse.body.isEmpty()) {
        String statusStr = extractValue(m_lastResponse.body, "devStatus");
        if (!statusStr.isEmpty()) {
            uint16_t status = statusStr.toInt();
            return status;
        }
    }

    return m_lastResponse.statusCode;
}

HttpCode IoTeye::updateDeviceStatus() {
    String endpoint;
    endpoint += ENDPOINT_DEVICES;
    endpoint += "/" + m_token + DEVICE_STATUS_UPDATE;

    m_logger->log(DebugLogger::INFO,
                  "Updating device status with endpoint: " + endpoint);
    return sendRequest(HttpMethod::GET, endpoint).statusCode;
}

HttpCode IoTeye::getLastHttpCode() {
    log("Getting last HTTP code: " + String(m_lastResponse.statusCode));
    return HttpCode(m_lastResponse.statusCode);
}

String IoTeye::getLastResponse() {
    m_logger->log(DebugLogger::INFO,
                  "Getting last response: " + m_lastResponse.body);
    return m_lastResponse.body;
}

ioteye::Response IoTeye::sendRequest(HttpMethod method, const String &endpoint,
                                     const ioteye::Payload &payload) {
    if (!m_commInterface)
        return {"Communication interface is not provided!", HttpCode(-1)};
    String url = "http://";
    url += m_serverUrl;
    if (!endpoint.isEmpty())
        url += endpoint;
    else
        url += '/';

    String data;
    if (payload.isEmpty())
        data = payload.GetArgsString();

    m_logger->log(DebugLogger::INFO,
                  "Sending request to URL: " + url + " with data: " + data);
    m_lastResponse = m_commInterface->sendData(method, url, data);
    log(m_lastResponse.body);
    return m_lastResponse;
}

String IoTeye::extractValue(const String &responseText, const String &key) {
    int startPos = responseText.indexOf(key + '=');
    if (startPos == -1) {
        return String();
    }
    startPos += key.length() + 1;
    int endPos =
        responseText.indexOf(" \r\n", startPos);  // Find the end of the value
    if (endPos == -1) {
        return responseText.substring(startPos);
    }
    if (startPos >= endPos) {
        return String();  // Invalid positions
    }
    return responseText.substring(startPos, endPos);
}

void IoTeye::log(const String &message) {
    if (m_logger)
        m_logger->log(DebugLogger::INFO, message);
}

IoTeye iotEye;