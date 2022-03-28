#include <core/microfacet.h>
#include <core/reflection.h>

DAKKU_BEGIN

Float MicrofacetDistribution::pdf(const Vector3f &wo,
                                  const Vector3f &wh) const {
  DAKKU_ERR("unimplemented");
  return 0;
}

Float TrowbridgeReitzDistribution::evalD(const Vector3f &wh) const {
  Float tan2 = tan2Theta(wh);
  if (std::isinf(tan2)) return 0;
  return 1 / (PI * alphaX * alphaY * cos2Theta(wh) * cos2Theta(wh) *
              (1 + tan2Theta(wh) * (cos2Phi(wh) / (alphaX * alphaX) +
                                    sin2Phi(wh) / (alphaY * alphaY))));
}

Float TrowbridgeReitzDistribution::evalLambda(const Vector3f &w) const {
  Float tan2 = tan2Theta(w);
  if (std::isinf(tan2)) return 0;
  Float alpha =
      std::sqrt(cos2Phi(w) * alphaX * alphaX + sin2Phi(w) * alphaY * alphaY);
  return (-1 + std::sqrt(1 + alpha * alpha * tan2)) / 2;
}

Vector3f TrowbridgeReitzDistribution::sampleWh(const Vector3f &wo,
                                               const Point2f &u) const {
  Vector3f wh;
  if (!sampleVisibleArea) {
    Float cosTheta = 0, phi = (2 * PI) * u[1];
    if (alphaX == alphaY) {
      Float tanTheta2 = alphaX * alphaX * u[0] / (1.0f - u[0]);
      cosTheta = 1 / std::sqrt(1 + tanTheta2);
    } else {
      phi = std::atan(alphaY / alphaX *
                      std::tan(2 * PI * u[1] + static_cast<Float>(0.5) * PI));
      if (u[1] > 0.5) phi += PI;
      Float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
      const Float alphaX2 = alphaX * alphaX, alphaY2 = alphaY * alphaY;
      const Float alpha2 =
          1 / (cosPhi * cosPhi / alphaX2 + sinPhi * sinPhi / alphaY2);
      Float tanTheta2 = alpha2 * u[0] / (1 - u[0]);
      cosTheta = 1 / std::sqrt(1 + tanTheta2);
    }
    Float sinTheta =
        std::sqrt(std::max((Float)0., (Float)1. - cosTheta * cosTheta));
    wh = sphericalDirection(sinTheta, cosTheta, phi);
    if (!sameHemisphere(wo, wh)) wh = -wh;
  } else {
    bool flip = wo.z() < 0;
    //    wh = sampleTrowbridgeReitz(flip ? -wo : wo, alphaX, alphaY, u[0],
    //    u[1]);
    if (flip) wh = -wh;
  }
  return wh;
}
DAKKU_END
