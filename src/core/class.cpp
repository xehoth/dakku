#include <core/class.h>
#include <core/logger.h>

namespace dakku {
Class &Class::instance() {
  static Class _instance;
  return _instance;
}

bool Class::isDerivedFrom(std::string_view cur, std::string_view from) const {
  while (!cur.empty() && cur != from) {
    if (auto it = _classMap.find(cur); it != _classMap.end()) {
      cur = it->second.parent;
    } else {
      DAKKU_ERR("failed to find {} in class map", cur);
      return false;
    }
  }
  return cur == from;
}

bool Class::contains(std::string_view name) const {
  return _classMap.contains(name);
}
}  // namespace dakku