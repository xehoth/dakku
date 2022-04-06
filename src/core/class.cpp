#include <core/class.h>
#include <core/logger.h>

namespace dakku {
Class &Class::instance() {
  static Class _instance;
  return _instance;
}
}  // namespace dakku