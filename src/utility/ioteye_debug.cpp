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

#include "ioteye_debug.hpp"
namespace ioteye {
DebugLogger::DebugLogger(HardwareSerial* serial, int baudRate) {
    setSerial(serial, baudRate);
}

void DebugLogger::setSerial(HardwareSerial* serial, int baudRate) {
    if (serial == nullptr)
        return;
    m_serial = serial;
    if (!(*m_serial))
        m_serial->begin(baudRate);
}

void DebugLogger::setSerial(const DebugLogger& otherLogger) {
    m_serial = otherLogger.m_serial;
}

void DebugLogger::logln(LogLevel level, const char* message) {
    if (m_serial == nullptr)
        return;
    log(level, message);
    m_serial->println();
}

void DebugLogger::log(LogLevel level, const char* message) {
    if (m_serial == nullptr)
        return;
    printPrefix(getLevelString(level));
    m_serial->print(message);
}

const char* DebugLogger::getTimestamp(char* dest) {
    unsigned long currentTime = millis();
    unsigned long seconds = currentTime / 1000;
    unsigned long milliseconds = currentTime % 1000;
    sprintf(dest, "%lu.%03lu", seconds, milliseconds);
    return dest;
}

void DebugLogger::printPrefix(const char* levelStr) {
    char timestamp[20];
    m_serial->print('[');
    m_serial->print(getTimestamp(timestamp));
    m_serial->print("] [");
    m_serial->print(levelStr);
    m_serial->print("] ");
}

const char* DebugLogger::getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::STATUS:
            return "STATUS";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

void DebugLogger::printValue(const Printable& value) {
    m_serial->print(value);
}

}  // namespace ioteye

ioteye::DebugLogger LibLogger;