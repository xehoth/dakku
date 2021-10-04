//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_MATERIALS_MATTE_H_
#define DAKKU_INCLUDE_DAKKU_MATERIALS_MATTE_H_
#include <dakku/core/material.h>
#include <dakku/core/spectrum.h>
#include <dakku/core/reflection.h>
#include <memory>

namespace dakku {
class MatteMaterial : public Material {
 public:
  MatteMaterial(const std::shared_ptr<Texture<RGBSpectrum>> &Kd);
  void computeScatteringFunctions(SurfaceInteraction &si) const override;

 private:
  std::shared_ptr<Texture<RGBSpectrum>> Kd{nullptr};
  std::shared_ptr<BxDF> lambertian{nullptr};
};
}
#endif  // DAKKU_INCLUDE_DAKKU_MATERIALS_MATTE_H_
