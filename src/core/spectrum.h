#ifndef DAKKU_SRC_CORE_SPECTRUM_H_
#define DAKKU_SRC_CORE_SPECTRUM_H_
#include <core/vector_base.h>
#include <span>

DAKKU_BEGIN

inline void xyzToRgb(std::span<const Float, 3> xyz, std::span<Float, 3> rgb) {
  rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
  rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
  rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
}

inline void rgbToXyz(std::span<const Float, 3> rgb, std::span<Float, 3> xyz) {
  xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
  xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
  xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];
}

template <ArithmeticType T, size_t size>
class CoefficientSpectrumT
    : public VectorBase<Float, size, CoefficientSpectrumT<T, size>> {
 public:
  DAKKU_DECLARE_VECTOR_DERIVED(CoefficientSpectrumT, Float, size);

  using super::operator+=;

  [[nodiscard]] bool isBlack() const { return super::isZero(); }
};

class RgbSpectrum : public CoefficientSpectrum<3> {
 public:
  using CoefficientSpectrum<3>::CoefficientSpectrumT;

  static RgbSpectrum fromRgb(std::span<const Float, 3> rgb) {
    return RgbSpectrum(rgb[0], rgb[1], rgb[2]);
  }

  static RgbSpectrum fromXyz(std::span<const Float, 3> xyz) {
    RgbSpectrum ret;
    xyzToRgb(xyz, ret);
    return ret;
  }

  void toRgb(std::span<Float, 3> rgb) const {
    rgb[0] = (*this)[0];
    rgb[1] = (*this)[1];
    rgb[2] = (*this)[2];
  }

  void toXyz(std::span<Float, 3> xyz) const { rgbToXyz(*this, xyz); }
};

DAKKU_END

#endif  // DAKKU_SRC_CORE_SPECTRUM_H_
