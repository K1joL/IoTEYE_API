#include <string>

#include "ioteyeapi/ioteyeapi.hpp"

auto main() -> int
{
  auto const exported = exported_class {};

  return std::string("ioteyeapi") == exported.name() ? 0 : 1;
}
