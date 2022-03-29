#include <core/light.h>
#include <core/scene.h>

DAKKU_BEGIN

void Light::construct(LightFlags _flags, const dakku::Transform &_lightToWorld,
                      int _nSamples) {
  this->flags = _flags;
  this->nSamples = std::max(1, _nSamples);
  this->lightToWorld = _lightToWorld;
  this->worldToLight = inverse(this->lightToWorld);
}

void AreaLight::construct(const Transform &lightToWorld, int nSamples) {
  Light::construct(LightFlags::AREA, lightToWorld, nSamples);
}

bool VisibilityTester::unoccluded(const Scene &scene) const {
  return !scene.intersectP(_p0.spawnRayTo(_p1));
}
DAKKU_END
