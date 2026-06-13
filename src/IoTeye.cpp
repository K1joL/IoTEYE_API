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

IoTeye::IoTeye() {
}

IoTeye::~IoTeye() {
    if (m_selfRegistered) {
        if (!deleteDevice().isSuccess())
            m_logger.logln(LogLevel::STATUS,
                           "You have to delete device with token: ", m_token,
                           " manually!");
    }
}

void IoTeye::init() {
    if ((m_token[0] != '\0') && (m_epManager.getTemplatesCount() != 0)) {
        m_isReady = getDeviceStatus(m_token) < STATES::MAX_STATES;
    }

    m_preInit = false;
}

IoTeye& IoTeye::setToken(const char* token) {
    m_logger.logln(LogLevel::INFO, "Setting token: ", token);
    strncpy(m_token, token, MAX_TOKEN_LENGTH);
    m_token[MAX_TOKEN_LENGTH - 1] = '\0';
    return *this;
}

IoTeye& IoTeye::setServerUrl(const char* url) {
    m_logger.logln(LogLevel::INFO, "Setting server URL: ", url);
    strncpy(m_serverUrl, url, MAX_URL_LENGTH);
    m_serverUrl[MAX_URL_LENGTH - 1] = '\0';
    return *this;
}

IoTeye& IoTeye::setServerUrl(const char* host, int port) {
    char url[MAX_URL_LENGTH];
    snprintf(url, sizeof(url), "%s:%d", host, port);
    return setServerUrl(url);
}

IoTeye& IoTeye::setLogger(const DebugLogger& logger) {
    m_logger.setSerial(logger);
    return *this;
}

IoTeye& IoTeye::setSerial(HardwareSerial* serial) {
    if (serial != nullptr)
        m_logger.setSerial(serial);
    return *this;
}

IoTeye& IoTeye::setCommunicationInterface(
    ioteye::IIoTeyeCommunication* commInterface) {
    m_commInterface = commInterface;
    return *this;
}

void IoTeye::run(unsigned long updateInterval) {
    unsigned long currentTime = millis();

    // Check if it's time to update the device status
    if (currentTime - m_lastUpdateTime >= updateInterval) {
        m_logger.logln(LogLevel::INFO, "Updating device status");
        updateDeviceStatus();
        m_lastUpdateTime = currentTime;  // Reset the timer
    }
}

HttpCode IoTeye::createVirtualPin(const char* pinNumber, const char* dataType,
                                  const char* defaultData) {
    char endpoint[MAX_ENDPOINT_SIZE];
    m_epManager.getCreateVirtPinEndpoint(endpoint, m_token, pinNumber, dataType,
                                         defaultData);
    m_logger.logln(
        LogLevel::INFO,
        "Creating virtual pin with default data (str): ", defaultData);
    m_logger.logln(LogLevel::INFO,
                   "Creating virtual pin with endpoint: ", endpoint);
    return sendRequest(HttpMethod::POST, endpoint).statusCode;
}

HttpCode IoTeye::createVirtualPin(const char* pinNumber, const char* dataType,
                                  int defaultData) {
    m_logger.logln(
        LogLevel::INFO,
        "Creating virtual pin with default data (int): ", defaultData);
    char defDataStr[MAX_INT_STR_WIDTH];
    itoa(defaultData, defDataStr, 10);
    return createVirtualPin(pinNumber, dataType, defDataStr);
}

HttpCode IoTeye::createVirtualPin(const char* pinNumber, const char* dataType,
                                  double defaultData, uint8_t precision) {
    m_logger.logln(
        LogLevel::INFO,
        "Creating virtual pin with default data (double): ", defaultData);
    char defDataStr[MAX_DOUBLE_STR_WIDTH];
    snprintf(defDataStr, MAX_DOUBLE_STR_WIDTH, "%.*f", precision, defaultData);
    return createVirtualPin(pinNumber, dataType, defDataStr);
}

HttpCode IoTeye::writeVirtualPin(const char* pinNumber, const char* value) {
    char endpoint[MAX_ENDPOINT_SIZE];
    m_epManager.getUpdateVirtPinEndpoint(endpoint, m_token, pinNumber, value);
    m_logger.logln(LogLevel::INFO,
                   "Writing virtual pin with endpoint: ", endpoint);
    return sendRequest(HttpMethod::PUT, endpoint).statusCode;
}

HttpCode IoTeye::writeVirtualPin(const char* pinNumber, const double value,
                                 uint8_t precision) {
    char valueStr[MAX_DOUBLE_STR_WIDTH];
    snprintf(valueStr, MAX_DOUBLE_STR_WIDTH, "%.*f", precision, value);
    return writeVirtualPin(pinNumber, valueStr);
}

HttpCode IoTeye::writeVirtualPin(const char* pinNumber, const int value) {
    char valueStr[MAX_INT_STR_WIDTH];
    snprintf(valueStr, MAX_INT_STR_WIDTH, "%d", value);
    return writeVirtualPin(pinNumber, valueStr);
}

HttpCode IoTeye::deleteVirtualPin(const char* pinNumber) {
    char endpoint[MAX_ENDPOINT_SIZE];
    m_epManager.getDeleteVirtPinEndpoint(endpoint, m_token, pinNumber);
    m_logger.logln(LogLevel::INFO,
                   "Deleting virtual pin with endpoint: ", endpoint);
    return sendRequest(HttpMethod::DELETE, endpoint).statusCode;
}

const char* IoTeye::getVirtualPin(const char* pinNumber,
                                  const char* otherToken) {
    const char* tokenToUse =
        (otherToken != nullptr && otherToken[0] != '\0') ? otherToken : m_token;
    char endpoint[MAX_ENDPOINT_SIZE];
    m_epManager.getGetVirtPinEndpoint(endpoint, tokenToUse, pinNumber);
    m_logger.logln(LogLevel::INFO,
                   "Getting virtual pin with endpoint: ", endpoint);
    if (sendRequest(HttpMethod::GET, endpoint).statusCode.isSuccess())
        return extractValue(m_lastResponse.body.c_str(), "PinValue");
    return "";
}

uint16_t IoTeye::getDeviceStatus(const char* otherToken) {
    const char* token;
    if (otherToken == "")
        return UINT16_MAX;
    else
        token = otherToken;

    char endpoint[MAX_ENDPOINT_SIZE];
    m_epManager.getDeviceStatusEndpoint(endpoint, token);

    m_logger.logln(LogLevel::INFO,
                   "Getting device status with endpoint: ", endpoint);
    if (!sendRequest(HttpMethod::GET, endpoint).statusCode.isSuccess())
        return UINT16_MAX;
    if (m_lastResponse.body != "") {
        const char* statusStr =
            extractValue(m_lastResponse.body.c_str(), "devStatus");
        if (statusStr != "") {
            uint16_t status = atoi(statusStr);
            return status;
        }
    }

    return m_lastResponse.statusCode;
}

HttpCode IoTeye::updateDeviceStatus() {
    char endpoint[MAX_ENDPOINT_SIZE];
    m_epManager.getUpdateDeviceStatusEndpoint(endpoint, m_token);

    m_logger.logln(LogLevel::INFO,
                   "Updating device status with endpoint: ", endpoint);
    return sendRequest(HttpMethod::PUT, endpoint).statusCode;
}

HttpCode IoTeye::registerDevice() {
    if (m_selfRegistered)
        return HttpCode::NULL_CODE;
    char endpoint[MAX_ENDPOINT_SIZE];
    m_logger.logln(LogLevel::ERROR, "registerDevice;");
    m_epManager.getRegisterDeviceEndpoint(endpoint);

    m_logger.logln(LogLevel::STATUS, "Registering device...");
    m_logger.logln(LogLevel::INFO,
                   "Registering device with endpoint: ", endpoint);
    sendRequest(HttpMethod::POST, endpoint);

    if (m_lastResponse.statusCode.isSuccess()) {
        if (m_lastResponse.body != "") {
            strncpy(m_token, extractValue(m_lastResponse.body.c_str(), "token"),
                    MAX_TOKEN_LENGTH);
            m_token[MAX_TOKEN_LENGTH - 1] = '\0';
            m_logger.logln(LogLevel::STATUS,
                           "Device registered with token: ", m_token);
            m_selfRegistered = true;
        }
    } else {
        m_logger.logln(LogLevel::WARNING,
                       "Error while registering device:", m_lastResponse.body);
    }

    return m_lastResponse.statusCode;
}

HttpCode IoTeye::deleteDevice(const char* token) {
    char endpoint[MAX_ENDPOINT_SIZE];
    m_epManager.getDeleteDeviceEndpoint(endpoint, token);

    m_logger.logln(LogLevel::STATUS, "Deleting device with token: ", token);
    m_logger.logln(LogLevel::INFO, "Deleting device with endpoint: ", endpoint);
    sendRequest(HttpMethod::DELETE, endpoint);

    if (m_lastResponse.statusCode.isSuccess())
        m_logger.logln(LogLevel::STATUS, "Device deleted successfully");
    if (m_token == token) {
        m_token[0] = '\0';
        m_isReady = false;
        m_preInit = true;
        if (m_selfRegistered)
            m_selfRegistered = false;
    } else
        m_logger.logln(LogLevel::WARNING,
                       "Error while deleting device:", m_lastResponse.body);

    return m_lastResponse.statusCode;
}

HttpCode IoTeye::deleteDevice() {
    return deleteDevice(m_token);
}

HttpCode IoTeye::getEndpoints() {
    // 1. Send init request
    // 2. Parse Templates
    // 3. Set TemplatesCount
    ioteye::Response response = sendRequest(HttpMethod::GET, "/init");
    m_epManager.setTemplatesCount(ioteye::parseTemplates(
        response.body.c_str(), m_epManager.getEndpointTable()));
    return response.statusCode;
}

HttpCode IoTeye::getLastHttpCode() {
    return HttpCode(m_lastResponse.statusCode);
}

const char* IoTeye::getLastResponse() {
    return m_lastResponse.body.c_str();
}

ioteye::Response IoTeye::sendRequest(HttpMethod method, const char* endpoint,
                                     const ioteye::Payload& payload) {
    if (!m_isReady && !m_preInit) {
        m_logger.logln(LogLevel::STATUS, "IoTeye is not ready yet!");
        return {"", HttpCode(-1)};
    }
    if (!m_commInterface) {
        m_logger.logln(LogLevel::ERROR,
                       "Communication interface is not provided!");
        return {"", HttpCode(-2)};
    }

    char url[MAX_URL_LENGTH];
    buildUrl(url, endpoint);

    const char* data;
    if (payload.isEmpty())
        data = payload.GetArgsString().c_str();

    m_logger.logln(LogLevel::INFO, "Sending request to URL: ", url,
                   " with data: ", data);
    m_lastResponse = m_commInterface->sendData(method, url, data);
    m_logger.logln(LogLevel::INFO, "Response: ", m_lastResponse.body);
    return m_lastResponse;
}

const char* IoTeye::extractValue(const char* responseText, const char* key) {
    const char* startPos = strstr(responseText, key);
    if (startPos == nullptr)
        return "";
    startPos += strlen(key);
    if (*startPos != '=')
        return "";
    ++startPos;
    // Find the end of the value
    const char* endPos = strpbrk(startPos, " \r\n");
    if (endPos == nullptr)
        endPos = responseText + strlen(responseText);
    static char value[MAX_VALUE_LENGTH + 1];
    int valueLength = endPos - startPos;
    if (valueLength > MAX_VALUE_LENGTH)
        valueLength = MAX_COMMAND_LENGTH;
    strncpy(value, startPos, valueLength);
    value[valueLength] = '\0';
    return value;
}

void IoTeye::buildUrl(char* url, const char* endpoint) {
    strncpy(url, "http://", MAX_URL_LENGTH);
    url[MAX_URL_LENGTH - 1] = '\0';
    strncat(url, m_serverUrl, MAX_URL_LENGTH - strlen(url) - 1);
    url[MAX_URL_LENGTH - 1] = '\0';

    if (endpoint != nullptr && endpoint[0] != '\0') {
        strncat(url, endpoint, MAX_URL_LENGTH - strlen(url) - 1);
        url[MAX_URL_LENGTH - 1] = '\0';
    } else {
        strncat(url, "/", MAX_URL_LENGTH - strlen(url) - 1);
        url[MAX_URL_LENGTH - 1] = '\0';
    }
}

IoTeye iotEye;