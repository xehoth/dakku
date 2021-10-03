//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/reflection.h>
#include <dakku/core/sampling.h>
#include <dakku/core/interaction.h>

namespace dakku {

Float BxDF::pdf(const Vector3f &wo, const Vector3f &wi) const {
  return sameHemisphere(wo, wi) ? absCosTheta(wi) * INV_PI : 0;
}

RGBSpectrum BxDF::sample(const Vector3f &wo, Vector3f &wi, const Point2f &u,
                         Float &pdf) const {
  wi = cosineSampleHemisphere(u);
  if (wo.z < 0) wi.z *= -1;
  pdf = this->pdf(wo, wi);
  return this->f(wo, wi);
}

RGBSpectrum LambertianReflection::f(const Vector3f &, const Vector3f &) const {
  return r * INV_PI;
}

BSDF::BSDF(const SurfaceInteraction &si) : normal(Vector3f{si.n}) {
  if (std::abs(normal.x) > std::abs(normal.z)) {
    binormal.x = -normal.y;
    binormal.y = normal.x;
    binormal.z = 0;
  } else {
    binormal.x = 0;
    binormal.y = -normal.z;
    binormal.z = normal.y;
  }
  binormal = normalize(binormal);
  tangent = cross(binormal, normal);
}

RGBSpectrum BSDF::f(const Vector3f &woW, const Vector3f &wiW) const {
  Vector3f wo = worldToLocal(woW), wi = worldToLocal(wiW);
  if (wo.z == 0) return RGBSpectrum{0};
  return bxdf->f(wo, wi);
}

Float BSDF::pdf(const Vector3f &woW, const Vector3f &wiW) const {
  if (!bxdf) return 0;
  Vector3f wo = worldToLocal(woW), wi = worldToLocal(wiW);
  if (wo.z == 0) return 0;
  return bxdf->pdf(wo, wi);
}

RGBSpectrum BSDF::sample(const Vector3f &woW, Vector3f &wiW,
                   const Point2f &u, Float &pdf) const {
  Vector3f wi, wo = worldToLocal(woW);
  if (wo.z == 0) return RGBSpectrum{0};
  RGBSpectrum f = bxdf->sample(wo, wi, u, pdf);
  if (pdf == 0) {
    return RGBSpectrum{0};
  }
  wiW = localToWorld(wi);
  return f;
}
}  // namespace dakku