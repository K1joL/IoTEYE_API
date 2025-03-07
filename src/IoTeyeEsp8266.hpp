#ifndef IOTEYE_ESP8266_HPP
#define IOTEYE_ESP8266_HPP

#ifndef ESP8266
#error This code is intended to run on the ESP8266 platform! \
Please check your Tools->Board setting.
#endif

#include "IoTeye.hpp"
#include "utility/communication/ioteye_esp8266.hpp"
#include "utility/ioteye_builder.hpp"

namespace ioteye {
class IoTeyeBuilderEsp8266 : public ioteye::IIoTeyeBuilder {
    void begin() override;
};

}  // namespace ioteye

extern ioteye::IoTeyeBuilderEsp8266 IoTeyeSettings;

#endif  // IOTEYE_ESP8266_HPP