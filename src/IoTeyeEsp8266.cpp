#include "IoTeyeEsp8266.hpp"
namespace ioteye {
void IoTeyeBuilderEsp8266::begin() {
    ESP8266Comm.begin(m_ssid, m_password);
    iotEye.setServerUrl(m_serverUrl)
            .setToken(m_token)
            .setLogger(m_serial)
            .setCommunicationInterface(&ESP8266Comm);
}
}  // namespace ioteye
ioteye::IoTeyeBuilderEsp8266 IoTeyeSettings;