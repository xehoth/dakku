//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/primitive.h>
#include <cassert>

namespace dakku {

void Aggregate::computeScatteringFunctions(SurfaceInteraction &isect) const {
  // should not be called
  assert(false);
}
GeometricPrimitive::GeometricPrimitive(const std::shared_ptr<Shape> &shape)
    : shape(shape) {}

bool GeometricPrimitive::intersect(const Ray &r,
                                   SurfaceInteraction &isect) const {
  return false;
}

bool GeometricPrimitive::occluded(const Ray &r) const { return false; }

void GeometricPrimitive::computeScatteringFunctions(
    SurfaceInteraction &isect) const {}
}  // namespace dakku