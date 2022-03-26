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

/**
 * convert degrees to radians
 * @param deg given degree
 * @return the radian of deg
 */
inline Float radians(Float deg) { return PI / 180 * deg; }

/**
 * convert radians to degrees
 * @param rad the given radian
 * @return the degree of rad
 */
inline Float degrees(Float rad) { return 180 / PI * rad; }

inline Float gammaCorrect(Float value) {
  if (value <= static_cast<Float>(0.0031308))
    return static_cast<Float>(12.92) * value;
  return static_cast<Float>(1.055) *
             std::pow(value, static_cast<Float>(1.0 / 2.4)) -
         static_cast<Float>(0.055);
}

inline Float inverseGammaCorrect(Float value) {
  if (value <= static_cast<Float>(0.04045))
    return value / static_cast<Float>(12.92);
  return std::pow(
      (value + static_cast<Float>(0.055)) / static_cast<Float>(1.055),
      static_cast<Float>(2.4));
}

DAKKU_END

#endif  // DAKKU_SRC_CORE_MATH_H_
