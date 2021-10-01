//
// Created by xehoth on 2021/10/1.
//
#include <dakku/core/geometry.h>

namespace dakku {

Ray::Ray(const Point3f &o, const Vector3f &d, Float tMax)
    : o(o), d(normalize(d)), tMax(tMax) {}

Point3f Ray::operator()(Float t) const { return o + d * t; }
}  // namespace dakku