#ifndef DAKKU_SRC_CORE_API_H_
#define DAKKU_SRC_CORE_API_H_
#include <core/fwd.h>
#include <core/logger.h>
#include <core/class.h>
#include <core/object.h>
#include <core/sobject.h>
#include <core/film.h>
#include <core/integrator.h>
#include <core/light.h>
#include <core/material.h>
#include <core/primitive.h>
#include <core/sampler.h>
#include <core/shape.h>
#include <core/state.h>
#include <core/texture.h>

DAKKU_BEGIN
namespace core {
/**
 * initialize dakku::core
 */
static void init() {
  logger::init();
  DAKKU_REGISTER_CLASS(Object);
  DAKKU_REGISTER_CLASS(SerializableObject);
  DAKKU_REGISTER_CLASS(Camera);
  DAKKU_REGISTER_CLASS(ProjectiveCamera);
  DAKKU_REGISTER_CLASS(Film);
  DAKKU_REGISTER_CLASS(Filter);
  DAKKU_REGISTER_CLASS(Integrator);
  DAKKU_REGISTER_CLASS(SamplerIntegrator);
  DAKKU_REGISTER_CLASS(Light);
  DAKKU_REGISTER_CLASS(AreaLight);
  DAKKU_REGISTER_CLASS(Material);
  DAKKU_REGISTER_CLASS(Primitive);
  DAKKU_REGISTER_CLASS(GeometricPrimitive);
  DAKKU_REGISTER_CLASS(Aggregate);
  DAKKU_REGISTER_CLASS(Sampler);
  DAKKU_REGISTER_CLASS(PixelSampler);
  DAKKU_REGISTER_CLASS(Scene);
  DAKKU_REGISTER_CLASS(Shape);
  DAKKU_REGISTER_CLASS(RenderState);
}
}  // namespace core
DAKKU_END
#endif  // DAKKU_SRC_CORE_API_H_
