//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/reflection.h>
#include <dakku/core/sampling.h>

namespace dakku {

Float BxDF::pdf(const Vector3f &wo, const Vector3f &wi) const {
  return sameHemisphere(wo, wi) ? absCosTheta(wi) * INV_PI : 0;
}

RGBSpectrum BxDF::sample(const Vector3f &wo, Vector3f &wi,
                         const Point2f &u, Float &pdf) const {
  wi = cosineSampleHemisphere(u);
  if (wo.z < 0) wi.z *= -1;
  pdf = this->pdf(wo, wi);
  return this->f(wo, wi);
}

RGBSpectrum LambertianReflection::f(const Vector3f &, const Vector3f &) const {
  return r * INV_PI;
}
}  // namespace dakku