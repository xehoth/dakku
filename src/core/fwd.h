#ifndef DAKKU_SRC_CORE_FWD_H_
#define DAKKU_SRC_CORE_FWD_H_
#include <defines.h>
#include <type_traits>

#if !defined(DAKKU_BEGIN)
#define DAKKU_BEGIN namespace dakku {
#endif

#if !defined(DAKKU_END)
#define DAKKU_END }
#endif

DAKKU_BEGIN

#if !defined(DAKKU_EXPORT_CORE)
#if DAKKU_BUILD_MODULE == DAKKU_MODULE_CORE
#define DAKKU_EXPORT_CORE DAKKU_EXPORT
#else
#define DAKKU_EXPORT_CORE DAKKU_IMPORT
#endif
#endif

#ifdef DAKKU_FLOAT_AS_DOUBLE
using Float = double;
#else
using Float = float;
#endif

template <typename T>
concept ArithmeticType = std::is_arithmetic_v<std::decay_t<T>>;

// abstraction & helpers
template <typename T>
class Singleton;
class Object;
class Class;
template <typename T>
concept ObjectType = std::is_base_of_v<Object, T>;

// serialization
class InputStream;
class OutputStream;
class FileInputStream;
class FileOutputStream;
struct Serializable;
struct SerializableObject;
struct RelativeRoot;

class Film;
class Filter;
class RenderState;
DAKKU_END
#endif  // DAKKU_SRC_CORE_FWD_H_