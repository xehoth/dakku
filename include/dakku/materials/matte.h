//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_MATERIALS_MATTE_H_
#define DAKKU_INCLUDE_DAKKU_MATERIALS_MATTE_H_
#include <dakku/core/material.h>
#include <dakku/core/spectrum.h>

namespace dakku {
class MatteMaterial : public Material {
 public:
  MatteMaterial(const RGBSpectrum &Kd);
  void computeScatteringFunctions(SurfaceInteraction &si) const override;

 private:
  RGBSpectrum Kd;
};
}
#endif  // DAKKU_INCLUDE_DAKKU_MATERIALS_MATTE_H_
