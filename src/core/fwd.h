#ifndef DAKKU_CORE_FWD_H_
#define DAKKU_CORE_FWD_H_
#include <platform.h>
#include <type_traits>
#include <limits>
#include <numbers>
#include <cmath>

namespace dakku {

#if DAKKU_BUILD_MODULE != DAKKU_CORE_MODULE
#define DAKKU_EXPORT_CORE DAKKU_IMPORT
#else
#define DAKKU_EXPORT_CORE DAKKU_EXPORT
#endif

/// infinity
static constexpr float INF = std::numeric_limits<float>::infinity();

/// PI
static constexpr float PI = std::numbers::pi_v<float>;

/**
 * @brief concept: `T` is an arithemetic type
 *
 * @tparam T typename
 */
template <typename T>
concept ArithmeticType = std::is_arithmetic_v<std::decay_t<T>>;

/**
 * @brief check whether `x` is not a number
 *
 * @tparam T type (floating point)
 * @param x given value
 * @return whether `x` is nan
 */
template <typename T>
requires std::is_floating_point_v<T> DAKKU_INLINE bool isNaN(T x) {
  return std::isnan(x);
}

/**
 * @brief check whether `x` is not a number
 *
 * @tparam T type (integer)
 * @return false (all integers are not nans)
 */
template <typename T>
requires std::is_integral_v<T> DAKKU_INLINE bool isNaN(T) { return false; }

/**
 * @brief convert degrees to radians
 *
 * @param deg given degree
 * @return the radian of deg
 */
inline float radians(float deg) { return PI / 180.0f * deg; }

/**
 * @brief convert radians to degrees
 *
 * @param rad the given radian
 * @return the degree of rad
 */
inline float degrees(float rad) { return 180.0f / PI * rad; }

enum class PropertyType;
class Property;
class Class;
class Object;
struct Matrix4x4;
/**
 * @brief object type concept
 *
 */
template <typename T>
concept ObjectType = std::is_base_of_v<Object, T>;
class Transform;
class Ray;
class RayDifferential;
}  // namespace dakku
#endif