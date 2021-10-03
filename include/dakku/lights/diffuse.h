//
// Created by xehoth on 2021/10/3.
//

#ifndef DAKKU_INCLUDE_DAKKU_LIGHTS_DIFFUSE_H_
#define DAKKU_INCLUDE_DAKKU_LIGHTS_DIFFUSE_H_
#include <dakku/core/dakku.h>
#include <dakku/core/light.h>
#include <dakku/core/spectrum.h>
namespace dakku {

class DiffuseAreaLight : public AreaLight {
 public:
  DiffuseAreaLight(const Transform &lightToWorld, const RGBSpectrum &Le,
                   const std::shared_ptr<Shape> &shape, bool twoSided = false);
  RGBSpectrum emit(const Interaction &intr, const Vector3f &w) const override;
  RGBSpectrum sampleLi(const Interaction &ref, const Point2f &u, Vector3f &wi,
                       Float &pdf, VisibilityTester &vis) const override;

 protected:
  const RGBSpectrum emitL;
  std::shared_ptr<Shape> shape{nullptr};
  const bool twoSided;
  const Float area;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_LIGHTS_DIFFUSE_H_
