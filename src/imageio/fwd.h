#ifndef DAKKU_IMAGEIO_FWD_H_
#define DAKKU_IMAGEIO_FWD_H_
#include <platform.h>
#include <cmath>

namespace dakku {
#if DAKKU_BUILD_MODULE != DAKKU_IMAGEIO_MODULE
#define DAKKU_EXPORT_IMAGEIO DAKKU_IMPORT
#else
#define DAKKU_EXPORT_IMAGEIO DAKKU_EXPORT
#endif

/**
 * @brief gamma correction
 *
 */
inline float gammaCorrect(float value) {
  if (value <= 0.0031308f) return 12.92f * value;
  return 1.055f * std::pow(value, 1.0f / 2.4f) - 0.055f;
}

/**
 * @brief inverse gamma correction
 *
 */
inline float inverseGammaCorrect(float value) {
  if (value <= 0.04045f) return value / 12.92f;
  return std::pow((value + 0.055f) / 1.055f, 2.4f);
}
}  // namespace dakku
#endif