/**
 * the reflection use a local coordinate system, whether the normal is aligned
 * with +z axis (left handed)
 */
#ifndef DAKKU_SRC_CORE_REFLECTION_H_
#define DAKKU_SRC_CORE_REFLECTION_H_
#include <core/vector.h>
#include <core/spectrum.h>
DAKKU_BEGIN

inline Float cosTheta(const Vector3f &w) { return w.z(); }
inline Float absCosTheta(const Vector3f &w) { return std::abs(w.z()); }
inline bool sameHemisphere(const Vector3f &w, const Vector3f &n) {
  return w.z() * n.z() > 0;
}
inline bool sameHemisphere(const Vector3f &w, const Normal3f &n) {
  return w.z() * n.z() > 0;
}

/**
 * fresnel reflectance of dielectric
 * @param cosThetaI incident cos theta
 * @param etaI index of refraction (incident side)
 * @param etaT index of refraction (transmit side)
 * @return the fresnel reflectance f_r
 */
Float frDielectric(Float cosThetaI, Float etaI, Float etaT);

/**
 * fresnel reflectance of conductor
 * * sin(thetaI) / sin(thetaT) = (etaT - i kT) / etaI
 * eta = etaT / etaI
 * etaK = kT / etaI
 * @param cosThetaI incident cos theta
 * @k kT
 */
Spectrum frConductor(Float cosThetaI, const Spectrum &etaI,
                     const Spectrum &etaT, const Spectrum &k);

/**
 * calculate the refraction ray
 * @param wi incident direction
 * @param n the normal
 * @param eta etaI / etaT
 * @param wt refraction direction
 * @return whether it refracts
 */
inline bool refract(const Vector3f &wi, const Normal3f &n, Float eta,
                    Vector3f &wt) {
  // compute cosTheta using Snell's law
  Float cosThetaI = n.dot(wi);
  Float sin2ThetaI = std::max(static_cast<Float>(0), 1 - cosThetaI * cosThetaI);
  Float sin2ThetaT = eta * eta * sin2ThetaI;

  // handle total internal reflection for transmission
  if (sin2ThetaT >= 1) return false;
  Float cosThetaT = std::sqrt(1 - sin2ThetaT);
  wt = eta * -wi + (eta * cosThetaI - cosThetaT) * Vector3f(n);
  return true;
}

// BSDF Declarations
enum class BxDFType : int {
  BSDF_REFLECTION = 1 << 0,
  BSDF_TRANSMISSION = 1 << 1,
  BSDF_DIFFUSE = 1 << 2,
  BSDF_GLOSSY = 1 << 3,
  BSDF_SPECULAR = 1 << 4,
  BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION |
             BSDF_TRANSMISSION,
};

inline BxDFType operator&(const BxDFType &a, const BxDFType &b) {
  return static_cast<BxDFType>(static_cast<int>(a) & static_cast<int>(b));
}

inline BxDFType operator|(const BxDFType &a, const BxDFType &b) {
  return static_cast<BxDFType>(static_cast<int>(a) | static_cast<int>(b));
}

class BSDF {
 public:
  /**
   * @param si geometry information
   * @param eta relative index of refraction over the boundary
   */
  explicit BSDF(const SurfaceInteraction &si, Float eta = 1);

  void add(BxDF *b) {
    DAKKU_CHECK(nBxDFs < MAX_BXDFS, "overflow: too many BxDFs!");
    bxdfs[nBxDFs++] = b;
  }
  [[nodiscard]] Vector3f worldToLocal(const Vector3f &v) const {
    return Vector3f(v.dot(ss), v.dot(ts), v.dot(ns));
  }
  [[nodiscard]] Vector3f localToWorld(const Vector3f &v) const {
    return ss * v.x() + ts * v.y() + Vector3f(ns) * v.z();
  }
  /**
   * calculate the number of components which match the flags
   */
  [[nodiscard]] int numComponents(BxDFType flags) const;
  /**
   * evaluate the BSDF with given flags
   */
  [[nodiscard]] Spectrum f(const Vector3f &woW, const Vector3f &wiW,
                           BxDFType flags = BxDFType::BSDF_ALL) const;
  /**
   * sample & evaluate the BSDF with given flags
   * @param[out] wi the sampled direction
   * @param[in] sample the random sample
   * @param[out] pdf the pdf
   */
  Spectrum sampleF(const Vector3f &wo, Vector3f &wi, const Point2f &sample,
                   Float &pdf, BxDFType type = BxDFType::BSDF_ALL,
                   BxDFType *sampledType = nullptr) const;
  [[nodiscard]] Float pdf(const Vector3f &wo, const Vector3f &wi,
                          BxDFType flags = BxDFType::BSDF_ALL) const;

  // relative index of refraction over the boundary
  const Float eta;

 private:
  // shading normal, geometric normal
  const Normal3f ns, ng;
  // orthonormal vectors (BSDF coordinate system)
  Vector3f ss, ts;
  int nBxDFs{};
  static constexpr int MAX_BXDFS = 8;
  BxDF *bxdfs[MAX_BXDFS]{};
};

class BxDF {
 public:
  virtual ~BxDF() = default;
  explicit BxDF(BxDFType type) : type(type) {}
  [[nodiscard]] bool matchesFlags(BxDFType t) const {
    return (type & t) == type;
  }
  /**
   * evaluate the BxDF
   */
  [[nodiscard]] virtual Spectrum f(const Vector3f &wo,
                                   const Vector3f &wi) const = 0;
  /**
   * sample & evaluate the BxDF
   * @param[out] wi the sampled direction
   * @param[in] sample the random sample
   * @param[out] pdf the pdf
   */
  virtual Spectrum sampleF(const Vector3f &wo, Vector3f &wi,
                           const Point2f &sample, Float &pdf,
                           BxDFType *sampledType) const;
  /**
   * pdf
   */
  [[nodiscard]] virtual Float pdf(const Vector3f &wo, const Vector3f &wi) const;

  const BxDFType type;
};

/**
 * Fresnel base class
 */
class Fresnel {
 public:
  virtual ~Fresnel() = default;
  [[nodiscard]] virtual Spectrum evaluate(Float cosThetaI) const = 0;
};

/**
 * Fresnel conductor
 */
class FresnelConductor : public Fresnel {
 public:
  explicit FresnelConductor(const Spectrum &etaI, const Spectrum &etaT,
                            const Spectrum &k)
      : etaI(etaI), etaT(etaT), k(k) {}
  [[nodiscard]] Spectrum evaluate(Float cosThetaI) const override;

 private:
  Spectrum etaI, etaT, k;
};

/**
 * Fresnel dielectric
 */
class FresnelDielectric : public Fresnel {
 public:
  explicit FresnelDielectric(Float etaI, Float etaT) : etaI(etaI), etaT(etaT) {}
  [[nodiscard]] Spectrum evaluate(Float cosThetaI) const override;

 private:
  Float etaI, etaT;
};

/**
 * No fresnel
 */
class FresnelNoOperation : public Fresnel {
 public:
  [[nodiscard]] Spectrum evaluate(Float cosThetaI) const override {
    return Spectrum{static_cast<Float>(1)};
  }
};

/**
 * lambertian reflection
 */
class LambertianReflection : public BxDF {
 public:
  explicit LambertianReflection(const Spectrum &r)
      : BxDF(BxDFType::BSDF_REFLECTION | BxDFType::BSDF_DIFFUSE), r(r) {}
  [[nodiscard]] Spectrum f(const Vector3f &wo,
                           const Vector3f &wi) const override;

 private:
  const Spectrum r;
};

/**
 * specular reflection
 */
class SpecularReflection : public BxDF {
 public:
  explicit SpecularReflection(const Spectrum &r, const Fresnel *fresnel)
      : BxDF(BxDFType::BSDF_REFLECTION | BxDFType::BSDF_SPECULAR),
        r(r),
        fresnel(fresnel) {}
  [[nodiscard]] Spectrum f(const Vector3f &wo,
                           const Vector3f &wi) const override {
    return Spectrum{0};
  }
  Spectrum sampleF(const Vector3f &wo, Vector3f &wi, const Point2f &sample,
                   Float &pdf, BxDFType *sampledType) const override;
  [[nodiscard]] Float pdf(const Vector3f &wo,
                          const Vector3f &wi) const override {
    return 0;
  }

 private:
  const Spectrum r;
  const Fresnel *fresnel;
};

/**
 * specular transmission
 */
class SpecularTransmission : public BxDF {
 public:
  explicit SpecularTransmission(const Spectrum &t, Float etaA, Float etaB)
      : BxDF(BxDFType::BSDF_TRANSMISSION | BxDFType::BSDF_SPECULAR),
        t(t),
        etaA(etaA),
        etaB(etaB),
        fresnel(etaA, etaB) {}

  [[nodiscard]] Spectrum f(const Vector3f &wo,
                           const Vector3f &wi) const override {
    return Spectrum{0};
  }
  [[nodiscard]] Float pdf(const Vector3f &wo,
                          const Vector3f &wi) const override {
    return 0;
  }
  Spectrum sampleF(const Vector3f &wo, Vector3f &wi, const Point2f &sample,
                   Float &pdf, BxDFType *sampledType) const override;

 private:
  const Spectrum t;
  const Float etaA;
  const Float etaB;
  const FresnelDielectric fresnel;
};

/**
 * Fresnel specular
 * combine specular reflection and specular transmission together
 */
class FresnelSpecular : public BxDF {
 public:
  explicit FresnelSpecular(const Spectrum &r, const Spectrum &t, Float etaA,
                           Float etaB)
      : BxDF(BxDFType::BSDF_SPECULAR | BxDFType::BSDF_REFLECTION |
             BxDFType::BSDF_TRANSMISSION),
        r(r),
        t(t),
        etaA(etaA),
        etaB(etaB) {}
  [[nodiscard]] Spectrum f(const Vector3f &wo, const Vector3f &wi) const override {
    return Spectrum{0};
  }
  [[nodiscard]] Float pdf(const Vector3f &wo, const Vector3f &wi) const override { return 0; }
  Spectrum sampleF(const Vector3f &wo, Vector3f &wi, const Point2f &sample,
                   Float &pdf, BxDFType *sampledType) const override;

 private:
  // reflection, transmission
  const Spectrum r, t;
  const Float etaA, etaB;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_REFLECTION_H_
