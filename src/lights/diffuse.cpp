//
// Created by xehoth on 2021/10/3.
//
#include <dakku/lights/diffuse.h>
#include <dakku/core/shape.h>

namespace dakku {

DiffuseAreaLight::DiffuseAreaLight(const Transform &lightToWorld,
                                   const RGBSpectrum &emitL,
                                   const std::shared_ptr<Shape> &shape,
                                   bool twoSided)
    : AreaLight(lightToWorld),
      emitL(emitL),
      shape(shape),
      twoSided(twoSided),
      area(shape->area()) {}

RGBSpectrum DiffuseAreaLight::emit(const Interaction &intr,
                                   const Vector3f &w) const {
  return (twoSided || dot(intr.n, w) > 0) ? emitL : RGBSpectrum(0);
}
RGBSpectrum DiffuseAreaLight::sampleLi(const Interaction &ref, const Point2f &u,
                                       Vector3f &wi, Float &pdf,
                                       VisibilityTester &vis) const {
  Interaction pShape = shape->sample(ref, u, pdf);
  if (pdf == 0 || (pShape.p - ref.p).lengthSquared() == 0) {
    pdf = 0;
    return RGBSpectrum{0};
  }
  Vector3f l = pShape.p - ref.p;
  wi = normalize(pShape.p - ref.p);
  pdf *= l.lengthSquared() / absDot(-wi, pShape.n);
  vis = VisibilityTester(ref, pShape);
  return this->emit(pShape, -wi);
}

}  // namespace dakku