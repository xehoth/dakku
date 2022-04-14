#include <core/reflection.h>
#include <core/spectrum.h>
#include <core/interaction.h>
#include <core/sampling.h>

namespace dakku {

float frDielectric(float cosThetaI, float etaI, float etaT) {
  cosThetaI = std::clamp<float>(cosThetaI, -1, 1);
  // potentially swap indices of refraction
  if (bool entering = cosThetaI > 0; !entering) {
    std::swap(etaI, etaT);
    cosThetaI = -cosThetaI;
  }
  // Snell's law -> cosThetaT
  float sinThetaI =
      std::sqrt(std::max(static_cast<float>(0), 1 - cosThetaI * cosThetaI));
  float sinThetaT = etaI / etaT * sinThetaI;

  // total internal reflection
  if (sinThetaT >= 1) return 1;

  float cosThetaT =
      std::sqrt(std::max(static_cast<float>(0), 1 - sinThetaT * sinThetaT));
  float rParallel = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                    ((etaT * cosThetaI) + (etaI * cosThetaT));
  float rPerp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
                ((etaI * cosThetaI) + (etaT * cosThetaT));
  return (rParallel * rParallel + rPerp * rPerp) / 2;
}


Spectrum frConductor(float cosThetaI, const Spectrum &etaI,
                     const Spectrum &etaT, const Spectrum &k) {
  cosThetaI = std::clamp<float>(cosThetaI, -1, 1);
  Spectrum eta = etaT / etaI;
  Spectrum etaK = k / etaI;
  float cosThetaI2 = cosThetaI * cosThetaI;
  float sinThetaI2 = 1 - cosThetaI2;
  Spectrum eta2 = eta * eta;
  Spectrum etaK2 = etaK * etaK;

  Spectrum t0 = eta2 - etaK2 - Spectrum(sinThetaI2);
  Spectrum a2plusb2 = sqrt(t0 * t0 + 4 * eta2 * etaK2);
  Spectrum t1 = a2plusb2 + Spectrum(cosThetaI2);
  Spectrum a = sqrt(0.5f * (a2plusb2 + t0));
  Spectrum t2 = 2 * cosThetaI * a;
  Spectrum Rs = (t1 - t2) / (t1 + t2);

  Spectrum t3 = cosThetaI2 * a2plusb2 + Spectrum(sinThetaI2 * sinThetaI2);
  Spectrum t4 = t2 * sinThetaI2;
  Spectrum Rp = Rs * (t3 - t4) / (t3 + t4);

  return static_cast<float>(0.5) * (Rp + Rs);
}

BSDF::BSDF(const SurfaceInteraction &si, float eta)
    : eta(eta),
      ns(si.shading.n),
      ng(si.n),
      ss(si.shading.dpdu.normalized()),
      ts(Vector3f(ns).cross(ss)) {}

Spectrum BSDF::f(const Vector3f &woW, const Vector3f &wiW,
                 BxDFType flags) const {
  Vector3f wi = worldToLocal(wiW), wo = worldToLocal(woW);
  if (wo.z() == 0) return Spectrum{0};
  // determine reflection / refraction
  bool reflect = wiW.dot(ng) * woW.dot(ng) > 0;
  Spectrum f;
  for (int i = 0; i < nBxDFs; ++i) {
    if (BxDF *b = bxdfs[i];
        b->matchesFlags(flags) &&
        ((reflect &&
          static_cast<int>(bxdfs[i]->type & BxDFType::BSDF_REFLECTION)) ||
         (!reflect &&
          static_cast<int>(bxdfs[i]->type & BxDFType::BSDF_TRANSMISSION)))) {
      f += b->f(wo, wi);
    }
  }
  return f;
}

Spectrum BSDF::sampleF(const Vector3f &woW, Vector3f &wiW, const Point2f &u,
                       float &pdf, BxDFType type, BxDFType *sampledType) const {
  // choose which BxDF to sample
  int matchingComps = numComponents(type);
  if (matchingComps == 0) {
    pdf = 0;
    if (sampledType) *sampledType = BxDFType(0);
    return Spectrum{0};
  }
  int comp = std::min(
      static_cast<int>(std::floor(u[0] * static_cast<float>(matchingComps))),
      matchingComps - 1);
  // get BxDF pointer for chosen component
  BxDF *bxdf{};
  int count = comp;
  for (int i = 0; i < nBxDFs; ++i) {
    if (bxdfs[i]->matchesFlags(type) && count-- == 0) {
      bxdf = bxdfs[i];
      break;
    }
  }
  DAKKU_CHECK(bxdf != nullptr, "bxdf is nullptr!");
  DAKKU_DEBUG("BSDF::sampleF chose comp = {} / matching = {}", comp,
              matchingComps);
  // remap bxdf sample u to [0, 1) ^ 2
  Point2f uRemapped(std::min(u[0] * static_cast<float>(matchingComps) -
                                 static_cast<float>(comp),
                             ONE_MINUS_EPSILON),
                    u[1]);

  // sample chosen bxdf
  Vector3f wi, wo = worldToLocal(woW);
  if (wo.z() == 0) return Spectrum{0};
  pdf = 0;
  if (sampledType) *sampledType = bxdf->type;
  Spectrum f = bxdf->sampleF(wo, wi, uRemapped, pdf, sampledType);
  if (pdf == 0) {
    if (sampledType) *sampledType = BxDFType(0);
    return Spectrum{0};
  }
  wiW = localToWorld(wi);

  // compute overall PDF with all matching BxDFs
  if (!static_cast<int>(bxdf->type & BxDFType::BSDF_SPECULAR) &&
      matchingComps > 1) {
    for (int i = 0; i < nBxDFs; ++i) {
      if (bxdfs[i] != bxdf && bxdfs[i]->matchesFlags(type)) {
        pdf += bxdfs[i]->pdf(wo, wi);
      }
    }
  }
  if (matchingComps > 1) pdf /= static_cast<float>(matchingComps);

  // compute value of BSDF for sampled direction
  if (!static_cast<int>(bxdf->type & BxDFType::BSDF_SPECULAR)) {
    bool reflect = wiW.dot(ng) * woW.dot(ng) > 0;
    f = Spectrum{0};
    for (int i = 0; i < nBxDFs; ++i) {
      if (bxdfs[i]->matchesFlags(type) &&
          ((reflect &&
            static_cast<int>(bxdfs[i]->type & BxDFType::BSDF_REFLECTION)) ||
           (!reflect &&
            static_cast<int>(bxdfs[i]->type & BxDFType::BSDF_TRANSMISSION)))) {
        f += bxdfs[i]->f(wo, wi);
      }
    }
  }
  return f;
}

int BSDF::numComponents(BxDFType flags) const {
  int num = 0;
  for (int i = 0; i < nBxDFs; ++i)
    num += static_cast<int>(bxdfs[i]->matchesFlags(flags));
  return num;
}

float BSDF::pdf(const Vector3f &woW, const Vector3f &wiW,
                BxDFType flags) const {
  if (nBxDFs == 0) return 0;
  Vector3f wo = worldToLocal(woW), wi = worldToLocal(wiW);
  if (wo.z() == 0) return 0;
  float pdf = 0;
  int matchingComps = 0;
  for (int i = 0; i < nBxDFs; ++i) {
    if (bxdfs[i]->matchesFlags(flags)) {
      ++matchingComps;
      pdf += bxdfs[i]->pdf(wo, wi);
    }
  }
  float v = matchingComps > 0 ? pdf / static_cast<float>(matchingComps) : 0;
  return v;
}

Spectrum BxDF::sampleF(const Vector3f &wo, Vector3f &wi, const Point2f &u,
                       float &pdf, BxDFType *) const {
  // cosine-sample the hemisphere, flipping the direction if necessary
  wi = cosineSampleHemisphere(u);
  if (wo.z() < 0) wi.z() = -wi.z();
  pdf = this->pdf(wo, wi);
  return f(wo, wi);
}

float BxDF::pdf(const Vector3f &wo, const Vector3f &wi) const {
  return sameHemisphere(wo, wi) ? absCosTheta(wi) * INV_PI : 0;
}

}  // namespace dakku