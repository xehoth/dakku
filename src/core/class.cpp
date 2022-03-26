#include <core/class.h>

DAKKU_BEGIN
Object *Class::create(std::string_view name) {
  if (auto it = _classMap.find(name); it != _classMap.end())
    return it->second();
  DAKKU_ERR("failed to find {} in class map", name);
  return nullptr;
}
DAKKU_END