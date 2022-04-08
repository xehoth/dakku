#include <core/property.h>

namespace dakku {

std::string Property::toString() const {
  nlohmann::json j = *this;
  return j.dump();
}

}  // namespace dakku