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

#ifndef IOTEYE_BUILDER_HPP
#define IOTEYE_BUILDER_HPP

#include <Arduino.h>

#include "utility/ioteye_debug.hpp"
#include "utility/ioteye_types.hpp"

namespace ioteye {
class IIoTeyeBuilder {
public:
    virtual ~IIoTeyeBuilder() = default;
    virtual void begin() = 0;
    IIoTeyeBuilder &setSSID(const String &ssid);
    IIoTeyeBuilder &setPassword(const String &pass);
    IIoTeyeBuilder &setToken(const String &token);
    IIoTeyeBuilder &setServerUrl(const String &serverUrl);
    IIoTeyeBuilder &setSSID(const char *ssid);
    IIoTeyeBuilder &setPassword(const char *pass);
    IIoTeyeBuilder &setToken(const char *token);
    IIoTeyeBuilder &setServerUrl(const char *serverUrl);
    IIoTeyeBuilder &setSerial(HardwareSerial *serial);
    IIoTeyeBuilder &setCommunicationSerial(HardwareSerial *serial);
    IIoTeyeBuilder &setSelfInit();

private:
    void setCommonString(char *destination, const char *source,
                         size_t maxLength);

protected:
    bool m_selfInit = false;
    char m_ssid[MAX_SSID_LENGTH];
    char m_password[MAX_PASSWORD_LENGTH];
    char m_token[MAX_TOKEN_LENGTH];
    char m_serverUrl[MAX_URL_LENGTH];
    HardwareSerial *m_serial = nullptr;
    HardwareSerial *m_commSerial = nullptr;
};
}  // namespace ioteye

#endif  // IOTEYE_BUILDER_HPP