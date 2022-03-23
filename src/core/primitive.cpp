#include <core/primitive.h>
#include <core/logger.h>
#include <core/material.h>
#include <core/interaction.h>

DAKKU_BEGIN

bool GeometricPrimitive::intersect(const Ray &, SurfaceInteraction &) const {
  DAKKU_ERR("intersect has not been implemented yet! Use aggregate instead");
  return false;
}
bool GeometricPrimitive::intersectP(const Ray &) const {
  DAKKU_ERR("intersect has not been implemented yet! Use aggregate instead");
  return false;
}

void GeometricPrimitive::computeScatteringFunctions(SurfaceInteraction &si,
                                                    MemoryArena &arena) const {
  if (material) material->computeScatteringFunctions(si, arena);
  DAKKU_CHECK(
      si.n.dot(si.shading.n) > 0,
      "geometric normal and shading normal are not in the same direction");
}

const AreaLight *GeometricPrimitive::getAreaLight() const { return areaLight; }

const Material *GeometricPrimitive::getMaterial() const { return material; }

void GeometricPrimitive::construct(const Shape *_shape,
                                   const Material *_material,
                                   const AreaLight *_areaLight) {
  this->shape = _shape;
  this->material = _material;
  this->areaLight = _areaLight;
}

void GeometricPrimitive::serialize(Json &, OutputStream *) const {
  DAKKU_ERR(
      "dakku now only support triangle mesh primitive, to be implemented!");
}

void GeometricPrimitive::unserialize(const Json &, InputStream *) {
  DAKKU_ERR(
      "dakku now only support triangle mesh primitive, to be implemented!");
}

const AreaLight *Aggregate::getAreaLight() const {
  DAKKU_ERR("use GeometricPrimitive::getAreaLight() instead!");
  return nullptr;
}

const Material *Aggregate::getMaterial() const {
  DAKKU_ERR("use GeometricPrimitive::getMaterial() instead!");
  return nullptr;
}

void Aggregate::computeScatteringFunctions(SurfaceInteraction &,
                                           MemoryArena &) const {
  DAKKU_ERR("use GeometricPrimitive::computeScatteringFunctions instead!");
}
DAKKU_END
