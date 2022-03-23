#include <core/sampling.h>

DAKKU_BEGIN
Point2f uniformSampleTriangle(const Point2f &u) {
  Float su0 = std::sqrt(u[0]);
  return Point2f(1 - su0, u[1] * su0);
}

Point2f concentricSampleDisk(const Point2f &u) {
  // map uniform random numbers to [-1, 1] ^ 2
  Point2f uOffset = 2 * u - Vector2f(1, 1);

  // handle degeneracy at the origin
  if (uOffset.x() == 0 && uOffset.y() == 0) return Point2f(0, 0);

  // apply concentric mapping to point
  Float theta, r;
  if (std::abs(uOffset.x()) > std::abs(uOffset.y())) {
    r = uOffset.x();
    theta = PI_OVER_4 * (uOffset.y() / uOffset.x());
  } else {
    r = uOffset.y();
    theta = PI_OVER_2 - PI_OVER_4 * (uOffset.x() / uOffset.y());
  }
  return r * Point2f(std::cos(theta), std::sin(theta));
}
DAKKU_END