#ifndef DAKKU_SRC_CORE_RAY_H_
#define DAKKU_SRC_CORE_RAY_H_
#include <core/fwd.h>
#include <core/vector.h>

DAKKU_BEGIN
class Ray {
 public:
  explicit Ray() = default;
  explicit Ray(const Point3f &o, const Vector3f &d, Float tMax = INF)
      : o(o), d(d), tMax(tMax) {}
  Point3f operator()(Float t) const { return o + d * t; }
  bool hasNaN() const { return o.hasNaN() || d.hasNaN() || isNaN(tMax); }
  friend std::ostream &operator<<(std::ostream &os, const Ray &r) {
    return os << "{o: " << r.o << ", d: " << r.d << ", tMax: " << r.tMax << "}";
  }
  Point3f o;
  Vector3f d;  // direction, note: this may not be normalized
  mutable Float tMax{INF};
};

class RayDifferential : public Ray {
 public:
  explicit RayDifferential() = default;
  explicit RayDifferential(const Point3f &o, const Vector3f &d,
                           Float tMax = INF)
      : Ray(o, d, tMax) {}
  explicit RayDifferential(const Ray &ray) : Ray(ray) {}
  bool hasNaN() const {
    return Ray::hasNaN() ||
           (hasDifferentials && (rxOrigin.hasNaN() || ryOrigin.hasNaN() ||
                                 rxDirection.hasNaN() || ryDirection.hasNaN()));
  }
  void scaleDifferentials(Float s) {
    // scale origin
    // o'' = o + s * (o' - o)
    rxOrigin = o + (rxOrigin - o) * s;
    ryOrigin = o + (ryOrigin - o) * s;
    // scale direction
    // d' = d + (o' - o)
    // d'' = d + s * (o' - o)
    rxDirection = d + (rxDirection - d) * s;
    ryDirection = d + (ryDirection - d) * s;
  }

  bool hasDifferentials{};
  Point3f rxOrigin, ryOrigin;
  Vector3f rxDirection, ryDirection;
};

/**
 * offset the ray origin accroding to SHADOW_EPS
 */
inline Point3f offsetRayOrigin(const Point3f &p, const Normal3f &n,
                               const Vector3f &w) {
  Vector3f offset = Vector3f(n) * SHADOW_EPS;
  if (w.dot(n) < 0) offset = -offset;
  Point3f po = p + offset;
  return po;
}
DAKKU_END
#endif  // DAKKU_SRC_CORE_RAY_H_
