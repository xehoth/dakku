#include <core/object.h>
#include <core/class.h>

namespace dakku {
std::string Object::getClassName() const { return getClassNameStatic(); }

std::string Object::toString() const { return getClassName(); }

std::string Object::getParentName() const { return getParentNameStatic(); }

DAKKU_EXPORT_OBJECT(Object);
}  // namespace dakku