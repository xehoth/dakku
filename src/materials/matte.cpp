//
// Created by xehoth on 2021/10/2.
//
#include <dakku/materials/matte.h>
#include <dakku/core/interaction.h>
#include <dakku/core/texture.h>
#include <new>

namespace dakku {

MatteMaterial::MatteMaterial(const std::shared_ptr<Texture<RGBSpectrum>> &Kd)
    : Kd(Kd),
      lambertian(std::make_shared<LambertianReflection>(RGBSpectrum{})) {}

void MatteMaterial::computeScatteringFunctions(SurfaceInteraction &si) const {
  si.bsdf = BSDF{si};
  RGBSpectrum r = Kd->evaluate(si);
  new (this->lambertian.get()) LambertianReflection(r);
  si.bsdf.add(this->lambertian);
}
}  // namespace dakku