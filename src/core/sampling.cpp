#include <core/sampling.h>

namespace dakku {
Point2f uniformSampleTriangle(const Point2f &u) {
  float su0 = std::sqrt(u[0]);
  return Point2f{1 - su0, u[1] * su0};
}

Point2f concentricSampleDisk(const Point2f &u) {
  // map uniform random numbers to [-1, 1] ^ 2
  Point2f uOffset = 2 * u - Vector2f(1, 1);

  // handle degeneracy at the origin
  if (uOffset.x() == 0 && uOffset.y() == 0) return Point2f{0, 0};

  // apply concentric mapping to point
  float theta, r;
  if (std::abs(uOffset.x()) > std::abs(uOffset.y())) {
    r = uOffset.x();
    theta = PI_OVER_4 * (uOffset.y() / uOffset.x());
  } else {
    r = uOffset.y();
    theta = PI_OVER_2 - PI_OVER_4 * (uOffset.x() / uOffset.y());
  }
  return r * Point2f(std::cos(theta), std::sin(theta));
}

Vector3f uniformSampleSphere(const Point2f &u) {
  float z = 1 - 2 * u[0];
  float r = std::sqrt(std::max(static_cast<float>(0), 1 - z * z));
  float phi = 2 * PI * u[1];
  return Vector3f{r * std::cos(phi), r * std::sin(phi), z};
}
}  // namespace dakku