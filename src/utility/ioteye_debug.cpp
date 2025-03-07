#include "ioteye_debug.hpp"
namespace ioteye {
void DebugLogger::log(LogLevel level, const String& message) {
    String levelStr;
    switch (level) {
        case INFO:
            levelStr = "INFO";
            break;
        case WARNING:
            levelStr = "WARNING";
            break;
        case ERROR:
            levelStr = "ERROR";
            break;
    }

    m_serial->print("[" + getTimestamp() + "] [" + levelStr + "] ");
    m_serial->println(message);
}

String DebugLogger::getTimestamp() {
    unsigned long currentTime = millis();
    unsigned long seconds = currentTime / 1000;
    unsigned long milliseconds = currentTime % 1000;
    return String(seconds) + "." + String(milliseconds);
}
}  // namespace ioteye

ioteye::DebugLogger LibLogger;