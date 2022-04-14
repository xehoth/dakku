#ifndef DAKKU_CORE_REFLECTION_H_
#define DAKKU_CORE_REFLECTION_H_
#include <core/vector.h>

namespace dakku {
inline float cosTheta(const Vector3f &w) { return w.z(); }
inline float cos2Theta(const Vector3f &w) { return w.z() * w.z(); }
inline float absCosTheta(const Vector3f &w) { return std::abs(w.z()); }
inline float sin2Theta(const Vector3f &w) {
  return std::max(static_cast<float>(0), 1 - cos2Theta(w));
}
inline float sinTheta(const Vector3f &w) { return std::sqrt(sin2Theta(w)); }
inline float tanTheta(const Vector3f &w) { return sinTheta(w) / cosTheta(w); }
inline float tan2Theta(const Vector3f &w) {
  return sin2Theta(w) / cos2Theta(w);
}
inline float cosPhi(const Vector3f &w) {
  float sinT = sinTheta(w);
  return sinT == 0 ? 1 : std::clamp<float>(w.x() / sinT, -1, 1);
}
inline float sinPhi(const Vector3f &w) {
  float sinT = sinTheta(w);
  return sinT == 0 ? 1 : std::clamp<float>(w.y() / sinT, -1, 1);
}
inline float cos2Phi(const Vector3f &w) { return cosPhi(w) * cosPhi(w); }
inline float sin2Phi(const Vector3f &w) { return sinPhi(w) * sinPhi(w); }
inline bool sameHemisphere(const Vector3f &w, const Vector3f &n) {
  return w.z() * n.z() > 0;
}
inline bool sameHemisphere(const Vector3f &w, const Normal3f &n) {
  return w.z() * n.z() > 0;
}

/**
 * @brief fresnel reflectance of conductor
 * sin(thetaI) / sin(thetaT) = (etaT - i kT) / etaI
 * eta = etaT / etaI
 * etaK = kT / etaI
 * @param cosThetaI incident cos theta
 * @param k kT
 */
float frDielectric(float cosThetaI, float etaI, float etaT);

/**
 * fresnel reflectance of conductor
 * * sin(thetaI) / sin(thetaT) = (etaT - i kT) / etaI
 * eta = etaT / etaI
 * etaK = kT / etaI
 * @param cosThetaI incident cos theta
 * @k kT
 */
Spectrum frConductor(float cosThetaI, const Spectrum &etaI,
                     const Spectrum &etaT, const Spectrum &k);

/**
 * @brief calculate reflection ray
 *
 */
inline Vector3f reflect(const Vector3f &wo, const Vector3f &n) {
  return -wo + 2 * wo.dot(n) * n;
}

/**
 * calculate the refraction ray
 * @param wi incident direction
 * @param n the normal
 * @param eta etaI / etaT
 * @param wt refraction direction
 * @return whether it refracts
 */
inline bool refract(const Vector3f &wi, const Normal3f &n, float eta,
                    Vector3f &wt) {
  // compute cosTheta using Snell's law
  float cosThetaI = n.dot(wi);
  float sin2ThetaI = std::max(static_cast<float>(0), 1 - cosThetaI * cosThetaI);
  float sin2ThetaT = eta * eta * sin2ThetaI;

  // handle total internal reflection for transmission
  if (sin2ThetaT >= 1) return false;
  float cosThetaT = std::sqrt(1 - sin2ThetaT);
  wt = eta * -wi + (eta * cosThetaI - cosThetaT) * Vector3f(n);
  return true;
}

/**
 * @brief bsdf types
 *
 */
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

inline BxDFType operator&(const BxDFType &a, int b) {
  return static_cast<BxDFType>(static_cast<int>(a) & b);
}

inline BxDFType operator&(int a, const BxDFType &b) {
  return static_cast<BxDFType>(a & static_cast<int>(b));
}

inline BxDFType operator|(const BxDFType &a, int b) {
  return static_cast<BxDFType>(static_cast<int>(a) | static_cast<int>(b));
}

inline BxDFType operator|(int a, const BxDFType &b) {
  return static_cast<BxDFType>(static_cast<int>(a) | static_cast<int>(b));
}

/**
 * @brief bsdf
 *
 */
class DAKKU_EXPORT_CORE BSDF {
 public:
  /**
   * @param si geometry information
   * @param eta relative index of refraction over the boundary
   */
  explicit BSDF(const SurfaceInteraction &si, float eta = 1);

  void add(BxDF *b) {
    DAKKU_CHECK(nBxDFs < MAX_BXDFS, "overflow: too many BxDFs!");
    bxdfs[nBxDFs++] = b;
  }
  [[nodiscard]] Vector3f worldToLocal(const Vector3f &v) const {
    return Vector3f{v.dot(ss), v.dot(ts), v.dot(ns)};
  }
  [[nodiscard]] Vector3f localToWorld(const Vector3f &v) const {
    return ss * v.x() + ts * v.y() + Vector3f(ns) * v.z();
  }
  /**
   * @brief calculate the number of components which match the flags
   */
  [[nodiscard]] int numComponents(BxDFType flags) const;
  /**
   * @brief evaluate the BSDF with given flags
   */
  [[nodiscard]] Spectrum f(const Vector3f &woW, const Vector3f &wiW,
                           BxDFType flags = BxDFType::BSDF_ALL) const;
  /**
   * @brief sample & evaluate the BSDF with given flags
   *
   * @param[out] wi the sampled direction
   * @param[in] sample the random sample
   * @param[out] pdf the pdf
   */
  Spectrum sampleF(const Vector3f &wo, Vector3f &wi, const Point2f &sample,
                   float &pdf, BxDFType type = BxDFType::BSDF_ALL,
                   BxDFType *sampledType = nullptr) const;
  [[nodiscard]] float pdf(const Vector3f &wo, const Vector3f &wi,
                          BxDFType flags = BxDFType::BSDF_ALL) const;

  // relative index of refraction over the boundary
  const float eta;

 private:
  // shading normal, geometric normal
  const Normal3f ns, ng;
  // orthonormal vectors (BSDF coordinate system)
  Vector3f ss, ts;
  int nBxDFs{};
  static constexpr int MAX_BXDFS = 8;
  BxDF *bxdfs[MAX_BXDFS]{};
};

/**
 * @brief BxDF
 * 
 */
class DAKKU_EXPORT_CORE BxDF {
 public:
  virtual ~BxDF() = default;
  explicit BxDF(BxDFType type) : type(type) {}
  [[nodiscard]] bool matchesFlags(BxDFType t) const {
    return (type & t) == type;
  }
  /**
   * @brief evaluate the BxDF
   */
  [[nodiscard]] virtual Spectrum f(const Vector3f &wo,
                                   const Vector3f &wi) const = 0;
  /**
   * @brief sample & evaluate the BxDF
   *
   * @param[out] wi the sampled direction
   * @param[in] sample the random sample
   * @param[out] pdf the pdf
   */
  virtual Spectrum sampleF(const Vector3f &wo, Vector3f &wi,
                           const Point2f &sample, float &pdf,
                           BxDFType *sampledType) const;
  /**
   * @brief pdf
   */
  [[nodiscard]] virtual float pdf(const Vector3f &wo, const Vector3f &wi) const;

  const BxDFType type;
};
}  // namespace dakku

/*! @page reflection
For BSDF, dakku uses the following coordinate system where the normal is aligned
to $(0, 0, 1)$:
![cornell_box](reflection/coordinate_system.svg)
*/
#endif