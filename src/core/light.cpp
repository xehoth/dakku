#include <core/light.h>

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
DAKKU_END
