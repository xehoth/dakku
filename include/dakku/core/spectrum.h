//
// Created by xehoth on 2021/10/1.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_SPECTRUM_H_
#define DAKKU_INCLUDE_DAKKU_CORE_SPECTRUM_H_
#include "dakku.h"
#include "geometry.h"

namespace dakku {

class RGBSpectrum : public TVector3<Float> {
 public:
  using TVector3<Float>::TVector3;
  template <typename U>
  requires std::is_convertible_v<U, Float> RGBSpectrum(U r, U g, U b)
      : TVector3<Float>(r, g, b) {}

  RGBSpectrum operator+(const RGBSpectrum &rhs) const;
  RGBSpectrum &operator+=(const RGBSpectrum &rhs);
  template <typename U>
  requires std::is_convertible_v<U, Float> RGBSpectrum operator*(U f) const {
    return RGBSpectrum(this->v * static_cast<Float>(f));
  }
  template <typename U>
  requires std::is_convertible_v<U, Float> RGBSpectrum &operator*=(U f) {
    this->v *= static_cast<Float>(f);
    return *this;
  }
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_SPECTRUM_H_
