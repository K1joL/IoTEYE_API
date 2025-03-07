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