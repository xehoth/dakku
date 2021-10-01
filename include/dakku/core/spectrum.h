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
  
  RGBSpectrum operator+(const RGBSpectrum &rhs) const;
  RGBSpectrum &operator+=(const RGBSpectrum &rhs);
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_SPECTRUM_H_
