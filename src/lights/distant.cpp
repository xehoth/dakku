#include <lights/distant.h>
#include <core/scene.h>
#include <core/bounds.h>

DAKKU_BEGIN
void DistantLight::construct(const Transform &lightToWorld,
                             const Spectrum &_data, const Vector3f &_wLight) {
  Light::construct(LightFlags::DELTA_DIRECTION, lightToWorld);
  this->data = _data;
  this->wLight = lightToWorld(_wLight).normalized();
}

Spectrum DistantLight::sampleLi(const Interaction &ref, const Point2f &u,
                                Vector3f &wi, Float &pdf,
                                VisibilityTester &vis) const {
  wi = wLight;
  pdf = 1;
  // choose a point outside the scene
  Point3f pOutside = ref.p + wLight * (2 * worldRadius);
  vis = VisibilityTester(ref, Interaction(pOutside));
  return data;
}

void DistantLight::preprocess(const Scene &scene) {
  scene.worldBound().boundingSphere(worldCenter, worldRadius);
}

void DistantLight::serialize(Json &json, OutputStream *stream) const {
  json["class"] = getClassNameStatic();
  DAKKU_SER_J(data);
  DAKKU_SER_J(wLight);
}

void DistantLight::unserialize(const Json &json, InputStream *stream) {
  DAKKU_UNSER_J(data);
  DAKKU_UNSER_J(wLight);
  Transform lightToWorld;
  this->construct(lightToWorld, data, wLight);
}

DAKKU_END