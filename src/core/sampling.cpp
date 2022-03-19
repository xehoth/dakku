#include <core/sampling.h>

DAKKU_BEGIN
Point2f uniformSampleTriangle(const Point2f &u) {
  Float su0 = std::sqrt(u[0]);
  return Point2f(1 - su0, u[1] * su0);
}
DAKKU_END