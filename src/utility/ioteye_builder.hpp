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

#include "IoTeye.hpp"
#include "utility/ioteye_communication.hpp"
#include "utility/ioteye_debug.hpp"

namespace ioteye {
class IIoTeyeBuilder {
public:
    virtual ~IIoTeyeBuilder() = default;
    virtual void begin() = 0;
    IIoTeyeBuilder &setSSID(const String &ssid);
    IIoTeyeBuilder &setPassword(const String &pass);
    IIoTeyeBuilder &setToken(const String &token);
    IIoTeyeBuilder &setServerUrl(const String &serverUrl);
    IIoTeyeBuilder &setLogger(DebugLogger *logger);
    IIoTeyeBuilder &setCommunicationLogger(DebugLogger *logger);
    IIoTeyeBuilder &setSelfInit();

protected:
    bool m_selfInit = false;
    String m_ssid;
    String m_password;
    String m_token;
    String m_serverUrl;
    DebugLogger *m_logger = nullptr;
    DebugLogger *m_commLogger = nullptr;
};
}  // namespace ioteye

#endif  // IOTEYE_BUILDER_HPP