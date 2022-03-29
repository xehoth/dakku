#include <core/microfacet.h>
#include <core/reflection.h>

DAKKU_BEGIN

Float MicrofacetDistribution::pdf(const Vector3f &wo,
                                  const Vector3f &wh) const {
  if (sampleVisibleArea) {
    return evalD(wh) * evalG1(wo) * wo.absDot(wh) / absCosTheta(wo);
  } else {
    return evalD(wh) * absCosTheta(wh);
  }
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

// TODO: rewrite this
static void sampleTrowbridgeReitz11(Float cosTheta, Float U1, Float U2,
                                    Float *slope_x, Float *slope_y) {
  // special case (normal incidence)
  if (cosTheta > .9999) {
    Float r = std::sqrt(U1 / (1 - U1));
    Float phi = static_cast<Float>(6.28318530718) * U2;
    *slope_x = r * std::cos(phi);
    *slope_y = r * std::sin(phi);
    return;
  }

  Float sinTheta = std::sqrt(
      std::max(static_cast<Float>(0), (Float)1 - cosTheta * cosTheta));
  Float tanTheta = sinTheta / cosTheta;
  Float a = 1 / tanTheta;
  Float G1 = 2 / (1 + std::sqrt(1.f + 1.f / (a * a)));

  // sample slope_x
  Float A = 2 * U1 / G1 - 1;
  Float tmp = 1.f / (A * A - 1.f);
  if (tmp > 1e10) tmp = 1e10;
  Float B = tanTheta;
  Float D = std::sqrt(
      std::max(Float(B * B * tmp * tmp - (A * A - B * B) * tmp), Float(0)));
  Float slope_x_1 = B * tmp - D;
  Float slope_x_2 = B * tmp + D;
  *slope_x = (A < 0 || slope_x_2 > 1.f / tanTheta) ? slope_x_1 : slope_x_2;

  // sample slope_y
  Float S;
  if (U2 > 0.5f) {
    S = 1.f;
    U2 = 2.f * (U2 - .5f);
  } else {
    S = -1.f;
    U2 = 2.f * (.5f - U2);
  }
  Float z = (U2 * (U2 * (U2 * 0.27385f - 0.73369f) + 0.46341f)) /
            (U2 * (U2 * (U2 * 0.093073f + 0.309420f) - 1.000000f) + 0.597999f);
  *slope_y = S * z * std::sqrt(1.f + *slope_x * *slope_x);

  DAKKU_CHECK(!std::isinf(*slope_y), "slope_y is inf");
  DAKKU_CHECK(!std::isnan(*slope_y), "slope_y is nan");
}

// TODO: rewrite this
static Vector3f sampleTrowbridgeReitz(const Vector3f &wi, Float alpha_x,
                                      Float alpha_y, Float U1, Float U2) {
  // 1. stretch wi
  Vector3f wiStretched =
      Vector3f(alpha_x * wi.x(), alpha_y * wi.y(), wi.z()).normalized();

  // 2. simulate P22_{wi}(x_slope, y_slope, 1, 1)
  Float slope_x, slope_y;
  sampleTrowbridgeReitz11(cosTheta(wiStretched), U1, U2, &slope_x, &slope_y);

  // 3. rotate
  Float tmp = cosPhi(wiStretched) * slope_x - sinPhi(wiStretched) * slope_y;
  slope_y = sinPhi(wiStretched) * slope_x + cosPhi(wiStretched) * slope_y;
  slope_x = tmp;

  // 4. unstretch
  slope_x = alpha_x * slope_x;
  slope_y = alpha_y * slope_y;

  // 5. compute normal
  return Vector3f(-slope_x, -slope_y, 1).normalized();
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
    wh = sampleTrowbridgeReitz(flip ? -wo : wo, alphaX, alphaY, u[0], u[1]);
    if (flip) wh = -wh;
  }
  return wh;
}
DAKKU_END
