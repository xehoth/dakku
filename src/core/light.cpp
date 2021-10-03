//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/light.h>
#include <dakku/core/scene.h>
#include <dakku/core/transform.h>
namespace dakku {

Light::Light(LightFlags flags, const Transform &lightToWorld)
    : flags(flags),
      lightToWorld(lightToWorld),
      worldToLight(inverse(lightToWorld)) {}

AreaLight::AreaLight(const Transform &lightToWorld)
    : Light(LightFlags::AREA, lightToWorld) {}

bool VisibilityTester::unoccluded(const Scene &scene) const {
  return !scene.occluded(p0.spawnRayTo(p1));
}
}  // namespace dakku