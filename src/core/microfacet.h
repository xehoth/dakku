#ifndef DAKKU_SRC_CORE_MICROFACET_H_
#define DAKKU_SRC_CORE_MICROFACET_H_
#include <core/vector.h>

DAKKU_BEGIN
/**
 * microfacet distribution
 */
class MicrofacetDistribution {
 public:
  virtual ~MicrofacetDistribution() = default;
  /**
   * evaluate D(wh)
   * @param wh surface normal
   * @return the result of D(wh)
   */
  [[nodiscard]] virtual Float evalD(const Vector3f &wh) const = 0;
  [[nodiscard]] virtual Float evalLambda(const Vector3f &w) const = 0;
  [[nodiscard]] Float evalG1(const Vector3f &w) const {
    return 1 / (1 + evalLambda(w));
  }
  [[nodiscard]] virtual Float evalG(const Vector3f &wo,
                                    const Vector3f &wi) const {
    return 1 / (1 + evalLambda(wo) + evalLambda(wi));
  }
  [[nodiscard]] virtual Vector3f sampleWh(const Vector3f &wo,
                                          const Point2f &u) const = 0;
  [[nodiscard]] Float pdf(const Vector3f &wo, const Vector3f &wh) const;

 protected:
  explicit MicrofacetDistribution(bool sampleVisibleArea)
      : sampleVisibleArea(sampleVisibleArea) {}

  const bool sampleVisibleArea;
};

/**
 * TrowbridgeReitzDistribution (GGX Distribution)
 */
class TrowbridgeReitzDistribution : public MicrofacetDistribution {
 public:
  explicit TrowbridgeReitzDistribution(Float alphaX, Float alphaY,
                                       bool sampleVisibleArea = true)
      : MicrofacetDistribution(sampleVisibleArea),
        alphaX(std::max(static_cast<Float>(0.001), alphaX)),
        alphaY(std::max(static_cast<Float>(0.001), alphaY)) {}
  [[nodiscard]] Float evalD(const Vector3f &wh) const override;
  [[nodiscard]] Float evalLambda(const Vector3f &w) const override;
  [[nodiscard]] Vector3f sampleWh(const Vector3f &wo,
                                  const Point2f &u) const override;

 private:
  Float alphaX, alphaY;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_MICROFACET_H_
