#pragma once

#if defined(_MSC_VER)
#if !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#endif

#include <type_traits>
#include <cmath>

namespace dakku {

template <typename T>
concept ArithmeticType = std::is_arithmetic_v<T>;

#ifdef DAKKU_USE_DOUBLE_AS_FLOAT
using Float = double;
#else
using Float = float;
#endif

constexpr Float INF = std::numeric_limits<Float>::infinity();

template <typename T>
inline bool isnan(const T x) {
  return std::isnan(x);
}

template <>
inline bool isnan(const int x) {
  return false;
}

}  // namespace dakku
