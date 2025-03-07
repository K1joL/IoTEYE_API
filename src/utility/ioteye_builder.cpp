#include "ioteye_builder.hpp"

namespace ioteye {

IIoTeyeBuilder& IIoTeyeBuilder::setSSID(const String& ssid) {
    m_ssid = ssid;
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setPassword(const String& pass) {
    m_password = pass;
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setToken(const String& token) {
    m_token = token;
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setServerUrl(const String& serverUrl) {
    m_serverUrl = serverUrl;
    return *this;
}

IIoTeyeBuilder& IIoTeyeBuilder::setLogger(HardwareSerial *serial) {
    m_serial = serial;
    return *this;
}

}  // namespace ioteye
