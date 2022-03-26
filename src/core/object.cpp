#include <core/object.h>

DAKKU_BEGIN

std::string Object::getClassName() const { return getClassNameStatic(); }
std::string Object::toString() const { return getClassName(); }

DAKKU_END
