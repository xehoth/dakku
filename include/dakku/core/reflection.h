//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_REFLECTION_H_
#define DAKKU_INCLUDE_DAKKU_CORE_REFLECTION_H_
#include <dakku/core/dakku.h>
#include <dakku/core/spectrum.h>
#include <dakku/core/geometry.h>
#include <memory>

namespace dakku {

inline bool sameHemisphere(const Vector3f &w, const Vector3f &wp) {
  return w.z * wp.z > 0;
}

inline Float absCosTheta(const Vector3f &w) { return std::abs(w.z); }

enum class BxDFType : int {
  BSDF_REFLECTION = 1 << 0,
  BSDF_TRANSMISSION = 1 << 1,
  BSDF_DIFFUSE = 1 << 2,
  BSDF_GLOSSY = 1 << 3,
  BSDF_SPECULAR = 1 << 4,
  BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION |
             BSDF_TRANSMISSION,
};

inline BxDFType operator|(const BxDFType &a, const BxDFType &b) {
  return static_cast<BxDFType>(static_cast<int>(a) | static_cast<int>(b));
}

class BxDF {
 public:
  virtual ~BxDF() = default;
  explicit BxDF(const BxDFType &type) : type(type) {}
  [[nodiscard]] bool matchesFlags(const BxDFType t) const {
    return (static_cast<int>(t) & static_cast<int>(type)) ==
           static_cast<int>(type);
  }
  virtual RGBSpectrum sample(const Vector3f &wo, Vector3f &wi,
                             const Point2f &sample, Float &pdf) const;
  [[nodiscard]] virtual RGBSpectrum f(const Vector3f &wo,
                                      const Vector3f &wi) const = 0;
  [[nodiscard]] virtual Float pdf(const Vector3f &wo, const Vector3f &wi) const;
  const BxDFType type;
};

class BSDF {
 public:
  BSDF() = default;
  explicit BSDF(const SurfaceInteraction &si);
  [[nodiscard]] RGBSpectrum f(const Vector3f &woW, const Vector3f &wiW) const;
  [[nodiscard]] Float pdf(const Vector3f &woW, const Vector3f &wiW) const;
  [[nodiscard]] RGBSpectrum sample(const Vector3f &wo, Vector3f &wi,
                                   const Point2f &u, Float &pdf) const;
  [[nodiscard]] Vector3f worldToLocal(const Vector3f &p) const {
    return Vector3f{dot(p, tangent), dot(p, binormal), dot(p, normal)};
  }
  [[nodiscard]] Vector3f localToWorld(const Vector3f &p) const {
    return tangent * p.x + binormal * p.y + normal * p.z;
  }
  [[nodiscard]] bool isValid() const { return bxdf != nullptr; }
  void add(std::shared_ptr<BxDF> bxdf) { this->bxdf = bxdf; }

 private:
  Vector3f normal, tangent, binormal;
  std::shared_ptr<BxDF> bxdf{nullptr};
};

class LambertianReflection : public BxDF {
 public:
  explicit LambertianReflection(const RGBSpectrum &r)
      : BxDF(BxDFType::BSDF_REFLECTION | BxDFType::BSDF_DIFFUSE), r(r) {}
  [[nodiscard]] RGBSpectrum f(const Vector3f &wo,
                              const Vector3f &wi) const override;

 private:
  const RGBSpectrum r;
};

}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_REFLECTION_H_
