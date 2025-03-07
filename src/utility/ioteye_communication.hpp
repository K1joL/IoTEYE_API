#ifndef IOTEYE_COMMUNICATION_HPP
#define IOTEYE_COMMUNICATION_HPP

#include "ioteye_types.hpp"

namespace ioteye {
class IIoTeyeCommunication {
public:
    virtual ~IIoTeyeCommunication() = default;

    virtual Response sendData(HttpMethod method, const String& url, const String& data = String()) = 0;
};
}  // namespace ioteye
#endif  // IOTEYE_COMMUNICATION_HPP