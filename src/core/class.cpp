#include <core/class.h>

DAKKU_BEGIN
Object *Class::create(std::string_view name) {
  if (auto it = _classMap.find(name); it != _classMap.end()) {
    if (it->second.constructor) {
      return it->second.constructor();
    } else {
      DAKKU_ERR("cannot construct {}, constructor is nullptr", name);
    }
  }
  DAKKU_ERR("failed to find {} in class map", name);
  return nullptr;
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
DAKKU_END