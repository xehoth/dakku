#ifndef DAKKU_CORE_SPECTRUM_H_
#define DAKKU_CORE_SPECTRUM_H_
#include <core/vector_base.h>

namespace dakku {

/**
 * @brief xyz to rgb
 *
 */
inline void xyzToRgb(std::span<const float, 3> xyz, std::span<float, 3> rgb) {
  rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
  rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
  rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
}

/**
 * @brief rgb to xyz
 *
 */
inline void rgbToXyz(std::span<const float, 3> rgb, std::span<float, 3> xyz) {
  xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
  xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
  xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];
}

/**
 * @brief coefficient spectrum
 *
 * @tparam S the number of spectrum samples
 * @tparam Derived derived type
 */
template <size_t S, typename Derived>
class CoefficientSpectrum : public VectorBase<float, S, Derived> {
 public:
  using VectorBase<float, S, Derived>::VectorBase;

  /**
   * @brief whether the spectrum is black
   *
   */
  [[nodiscard]] bool isBlack() const { return this->isZero(); }

  /**
   * @brief element-wise clamp
   *
   */
  [[nodiscard]] Derived clamp(float low = 0, float high = INF) const {
    Derived ret = static_cast<const Derived &>(*this);
    for (size_t i = 0; i < S; ++i) ret[i] = std::clamp(ret[i], low, high);
    return ret;
  }

  /**
   * @brief subtraction between spectrums
   *
   */
  friend Derived operator-(const Derived &a, const Derived &b) {
    Derived ret = a;
    ret -= b;
    return ret;
  }
};

/**
 * @brief spectrum that only use r, g, b
 *
 */
class RgbSpectrum : public CoefficientSpectrum<3, RgbSpectrum> {
 public:
  using CoefficientSpectrum<3, RgbSpectrum>::CoefficientSpectrum;

  static RgbSpectrum fromRgb(std::span<const float, 3> rgb) {
    return RgbSpectrum{rgb[0], rgb[1], rgb[2]};
  }

  static RgbSpectrum fromXyz(std::span<const float, 3> xyz) {
    RgbSpectrum ret;
    xyzToRgb(xyz, ret);
    return ret;
  }

  void toRgb(std::span<float, 3> rgb) const {
    rgb[0] = (*this)[0];
    rgb[1] = (*this)[1];
    rgb[2] = (*this)[2];
  }

  void toXyz(std::span<float, 3> xyz) const { rgbToXyz(*this, xyz); }
};
}  // namespace dakku
#endif