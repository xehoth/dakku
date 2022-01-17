#pragma once

#include <type_traits>
#include <cmath>

namespace dakku {

template <typename T>
concept ArithmeticType = std::is_arithmetic_v<T>;

template <ArithmeticType T>
inline bool isnan(const T x) {
  return std::isnan(x);
}

template <>
inline bool isnan(const int x) {
  return false;
}

constexpr float INF = std::numeric_limits<float>::infinity();

inline float lerp(float t, float v1, float v2) {
  return (1.0f - t) * v1 + t * v2;
}
}  // namespace dakku
