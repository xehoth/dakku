//
// Created by xehoth on 2021/10/2.
//
#include <dakku/materials/matte.h>
#include <dakku/core/interaction.h>

namespace dakku {

MatteMaterial::MatteMaterial(const RGBSpectrum &Kd)
    : Kd(Kd), lambertian(std::make_shared<LambertianReflection>(Kd)) {}

void MatteMaterial::computeScatteringFunctions(SurfaceInteraction &si) const {
  si.bsdf = BSDF{si};
  si.bsdf.add(this->lambertian);
}
}  // namespace dakku