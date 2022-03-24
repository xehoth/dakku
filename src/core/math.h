#ifndef DAKKU_SRC_CORE_MATH_H_
#define DAKKU_SRC_CORE_MATH_H_
#include <core/fwd.h>
#include <cmath>

DAKKU_BEGIN

template <typename T>
inline bool isNaN(const T x) {
  return std::isnan(x);
}

template <>
inline bool isNaN(const int x) {
  return false;
}

inline Float radians(Float deg) { return PI / 180 * deg; }

inline Float degrees(Float rad) { return 180 / PI * rad; }

DAKKU_END

#endif  // DAKKU_SRC_CORE_MATH_H_
