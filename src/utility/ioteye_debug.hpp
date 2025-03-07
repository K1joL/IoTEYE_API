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

#ifndef IOTEYE_DEBUG_HPP
#define IOTEYE_DEBUG_HPP

#include <Arduino.h>
namespace ioteye {
class DebugLogger {
public:
    enum LogLevel { INFO, WARNING, ERROR };

    DebugLogger() {
    }
    DebugLogger(HardwareSerial* serial){
        init(serial);
    }
    void init(HardwareSerial* serial, int baudRate = 115200) {
        if(serial == nullptr)
            return;
        if(m_serial == nullptr){
            m_serial = serial;
        if(!(*m_serial))
            m_serial->begin(baudRate);}
    }
    void log(LogLevel level, const String& message);

private:
    String getTimestamp();

private:
    HardwareSerial* m_serial = nullptr;
};

}  // namespace ioteye
extern ioteye::DebugLogger LibLogger;

#endif  // IOTEYE_DEBUG_HPP