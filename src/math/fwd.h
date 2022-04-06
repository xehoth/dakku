#ifndef DAKKU_MATH_FWD_H_
#define DAKKU_MATH_FWD_H_
#include <platform.h>
#include <type_traits>
#include <limits>
#include <numbers>
#include <cmath>

/**
 * @brief dakku namespace
 */
namespace dakku {

#if DAKKU_BUILD_MODULE != DAKKU_MATH_MODULE
#define DAKKU_EXPORT_MATH DAKKU_IMPORT
#else
#define DAKKU_EXPORT_MATH DAKKU_EXPORT
#endif

/// infinity
static constexpr float INF = std::numeric_limits<float>::infinity();

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

DAKKU_EXPORT_MATH void testMath();
}  // namespace dakku
#endif