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
#include <stdio.h>

namespace ioteye {
enum LogLevel { STATUS, INFO, WARNING, ERROR };
class DebugLogger {
public:
    DebugLogger() {
    }
    DebugLogger(HardwareSerial* serial, int baudRate = 115200);
    void setSerial(HardwareSerial* serial, int baudRate = 115200);
    void setSerial(const DebugLogger& otherLogger);
    void logln(LogLevel level, const char* message);
    template <typename... Args>
    void logln(LogLevel level, Args&&... args);
    void log(LogLevel level, const char* message);
    template <typename... Args>
    void log(LogLevel level, Args&&... args);

private:
    const char* getTimestamp(char* dest);
    void printPrefix(const char* levelStr);
    const char* getLevelString(LogLevel level);
    template <typename T, typename... Args>
    void printValues(T arg, Args... args);
    void printValues() {};
    void printValue(const Printable& value);
    template <typename T>
    void printValue(T value);

private:
    HardwareSerial* m_serial = nullptr;
};

template <typename... Args>
inline void DebugLogger::logln(LogLevel level, Args&&... args) {
    if (m_serial == nullptr)
        return;
    log(level, args...);
    m_serial->println();
}

template <typename... Args>
inline void DebugLogger::log(LogLevel level, Args&&... args) {
    if (m_serial == nullptr)
        return;
    printPrefix(getLevelString(level));
    printValues(args...);
}

template <typename T, typename... Args>
inline void DebugLogger::printValues(T arg, Args... args) {
    printValue(arg);
    printValues(args...);
}

template <typename T>
inline void DebugLogger::printValue(T value) {
    m_serial->print(value);
}

}  // namespace ioteye
extern ioteye::DebugLogger LibLogger;

#endif  // IOTEYE_DEBUG_HPP