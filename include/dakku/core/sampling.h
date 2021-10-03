//
// Created by xehoth on 2021/10/3.
//

#ifndef DAKKU_SRC_CORE_SAMPLING_H_
#define DAKKU_SRC_CORE_SAMPLING_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>
namespace dakku {

inline Point2f concentricSampleDisk(const Point2f &u) {
  // Map uniform random numbers to $[-1,1]^2$
  Point2f uOffset = u * static_cast<Float>(2) - Vector2f(1, 1);

  // Handle degeneracy at the origin
  if (uOffset.x == 0 && uOffset.y == 0) return Point2f{0, 0};

  // Apply concentric mapping to point
  Float theta, r;
  if (std::abs(uOffset.x) > std::abs(uOffset.y)) {
    r = uOffset.x;
    theta = PI_OVER4 * (uOffset.y / uOffset.x);
  } else {
    r = uOffset.y;
    theta = PI_OVER2 - PI_OVER4 * (uOffset.x / uOffset.y);
  }
  return Point2f{std::cos(theta), std::sin(theta)} * r;
}

inline Vector3f cosineSampleHemisphere(const Point2f &u) {
  Point2f d = concentricSampleDisk(u);
  Float z =
      std::sqrt(std::max(static_cast<Float>(0), 1 - d.x * d.x - d.y * d.y));
  return Vector3f{d.x, d.y, z};
}

inline Point2f uniformSampleTriangle(const Point2f &u) {
  Float su0 = std::sqrt(u.x);
  return Point2f{1 - su0, u.y * su0};
}
}  // namespace dakku
#endif  // DAKKU_SRC_CORE_SAMPLING_H_
