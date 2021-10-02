//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/primitive.h>
#include <dakku/core/material.h>
#include <cassert>

namespace dakku {

void Aggregate::computeScatteringFunctions(SurfaceInteraction &isect) const {
  // should not be called
  assert(false);
}
std::shared_ptr<const AreaLight> Aggregate::getAreaLight() const {
  assert(false);
  return nullptr;
}
std::shared_ptr<const Material> Aggregate::getMaterial() const {
  assert(false);
  return nullptr;
}

GeometricPrimitive::GeometricPrimitive(const std::shared_ptr<Shape> &shape)
    : shape(shape) {}

bool GeometricPrimitive::intersect(const Ray &r,
                                   SurfaceInteraction &isect) const {
  return false;
}

bool GeometricPrimitive::occluded(const Ray &r) const { return false; }

void GeometricPrimitive::computeScatteringFunctions(
    SurfaceInteraction &isect) const {
  if (material) {
    material->computeScatteringFunctions(isect);
  }
}

std::shared_ptr<const Material> GeometricPrimitive::getMaterial() const {
  return this->material;
}

std::shared_ptr<const AreaLight> GeometricPrimitive::getAreaLight() const {
  return this->areaLight;
}
}  // namespace dakku