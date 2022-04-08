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

bool Class::isBaseof(std::string_view cur, std::string_view other) const {
  while (!other.empty() && cur != other) {
    if (auto it = _classMap.find(other); it != _classMap.end()) {
      other = it->second.parent;
    } else {
      DAKKU_ERR("failed to find {} in class map", other);
      return false;
    }
  }
  return cur == other;
}

bool Class::contains(std::string_view name) const {
  return _classMap.contains(name);
}

Object *Class::create(std::string_view name, const Property &property) {
  if (auto it = _classMap.find(name); it != _classMap.end()) {
    if (it->second.constructor) {
      return it->second.constructor(property);
    } else {
      DAKKU_ERR("cannot construct {}, constructor is nullptr", name);
    }
  }
  DAKKU_ERR("failed to find {} in class map", name);
  return nullptr;
}
}  // namespace dakku