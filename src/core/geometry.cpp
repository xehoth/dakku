//
// Created by xehoth on 2021/10/1.
//
#include <dakku/core/geometry.h>

namespace dakku {

Ray::Ray(const Point3f &o, const Vector3f &d, Float tMax)
    : o(o), d(normalize(d)), tMax(tMax) {}

Point3f Ray::operator()(Float t) const { return o + d * t; }

Point3f offsetRayOrigin(const Point3f &p, const Normal3f &n, const Vector3f &w) {
  Vector3f offset = Vector3f(n) * SHADOW_EPS;
  if (dot(w, n) < 0) offset = -offset;
  Point3f po = p + offset;
  return po;
}
}  // namespace dakku