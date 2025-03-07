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
    IIoTeyeBuilder &setLogger(HardwareSerial *serial);

protected:
    String m_ssid;
    String m_password;
    String m_token;
    String m_serverUrl;
    HardwareSerial *m_serial = nullptr;
};
}  // namespace ioteye

#endif  // IOTEYE_BUILDER_HPP