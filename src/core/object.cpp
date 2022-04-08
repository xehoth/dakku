#include <core/object.h>
#include <core/class.h>

namespace dakku {
Object::Object(const Property &) {}

std::string Object::getClassName() const { return getClassNameStatic(); }

std::string Object::toString() const { return getClassName(); }

std::string Object::getParentName() const { return getParentNameStatic(); }

bool Object::isDerivedFrom(std::string_view name) const {
  return Class::instance().isDerivedFrom(this->getClassName(), name);
}

bool Object::isBaseOf(std::string_view name) const {
  return Class::instance().isBaseof(this->getClassName(), name);
}

DAKKU_EXPORT_OBJECT(Object);
}  // namespace dakku