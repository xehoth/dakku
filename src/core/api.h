/**
 * @file api.h
 * @brief this must be include only once in source
 * 
 */
#ifndef DAKKU_CORE_API_H_
#define DAKKU_CORE_API_H_
#include <core/object.h>
#include <core/serialization.h>

namespace dakku {

DAKKU_EXPORT_OBJECTS(Core, Object, InputStream, OutputStream, SerializableObject);
}  // namespace dakku
#endif