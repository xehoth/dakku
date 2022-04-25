#ifndef DAKKU_CORE_MATH_FUNC_H_
#define DAKKU_CORE_MATH_FUNC_H_
#include <core/fwd.h>

#include <cmath>

namespace dakku {

/**
 * @brief check whether `x` is not a number
 *
 * @tparam T type (floating point)
 * @param x given value
 * @return whether `x` is nan
 */
template <typename T>
requires std::is_floating_point_v<T> DAKKU_INLINE bool isnan(T x) {
  return std::isnan(x);
}

/**
 * @brief check whether `x` is not a number
 *
 * @tparam T type (integer)
 * @return false (all integers are not nans)
 */
template <typename T>
requires std::is_integral_v<T> DAKKU_INLINE bool isnan(T) { return false; }
}  // namespace dakku
#endif