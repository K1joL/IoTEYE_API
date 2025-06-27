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

#include "ioteye_builder.hpp"

namespace ioteye {

IIoTeyeBuilder& IIoTeyeBuilder::setSSID(const String& ssid) {
    setCommonString(m_ssid, ssid.c_str(), MAX_SSID_LENGTH);
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setPassword(const String& pass) {
    setCommonString(m_password, pass.c_str(), MAX_PASSWORD_LENGTH);
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setToken(const String& token) {
    setCommonString(m_token, token.c_str(), MAX_TOKEN_LENGTH);
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setServerUrl(const String& serverUrl) {
    setCommonString(m_serverUrl, serverUrl.c_str(), MAX_URL_LENGTH);
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setSSID(const char* ssid) {
    setCommonString(m_ssid, ssid, MAX_SSID_LENGTH);
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setPassword(const char* pass) {
    setCommonString(m_password, pass, MAX_PASSWORD_LENGTH);
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setToken(const char* token) {
    setCommonString(m_token, token, MAX_TOKEN_LENGTH);
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setServerUrl(const char* serverUrl) {
    setCommonString(m_serverUrl, serverUrl, MAX_URL_LENGTH);
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setSerial(HardwareSerial* logger) {
    m_serial = logger;
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setCommunicationSerial(HardwareSerial* logger) {
    m_commSerial = logger;
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setSelfInit() {
    m_selfInit = true;
    return *this;
}

void IIoTeyeBuilder::setCommonString(char* destination, const char* source,
                                     size_t maxLength) {
    size_t sourceLength = strlen(source);
    size_t copyLength = 0;
    if (sourceLength > maxLength - 1)
        copyLength = maxLength - 1;
    else
        copyLength = sourceLength;

    strncpy(destination, source, copyLength);
    destination[copyLength] = '\0';
}
}  // namespace ioteye
