//
// Created by xehoth on 2021/10/1.
//
#include <dakku/core/spectrum.h>

namespace dakku {

RGBSpectrum RGBSpectrum::operator+(const RGBSpectrum &rhs) const {
  return RGBSpectrum{this->v + rhs.v};
}

RGBSpectrum &RGBSpectrum::operator+=(const RGBSpectrum &rhs) {
  this->v += rhs.v;
  return *this;
}
RGBSpectrum RGBSpectrum::operator*(const RGBSpectrum &rhs) const {
  return RGBSpectrum{this->v * rhs.v};
}

RGBSpectrum RGBSpectrum::operator/(Float x) const {
  return RGBSpectrum{this->v / x};
}

RGBSpectrum &RGBSpectrum::operator*=(const RGBSpectrum &rhs) {
  this->v *= rhs.v;
  return *this;
}

}  // namespace dakku