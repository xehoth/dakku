#include <core/scene.h>
#include <core/interaction.h>
#include <core/ray.h>
#include <core/logger.h>
#include <core/primitive.h>
#include <core/light.h>

DAKKU_BEGIN

Scene::Scene(const Primitive *aggregate, const std::vector<Light *> &lights)
    : lights(lights), aggregate(aggregate) {
  for (const auto &light : lights) {
    light->preprocess(*this);
  }
}

bool Scene::intersect(const Ray &ray, SurfaceInteraction &isect) const {
  DAKKU_CHECK(ray.d != Vector3f(0, 0, 0), "ray direction is (0, 0, 0)");
  return aggregate->intersect(ray, isect);
}

bool Scene::intersectP(const Ray &ray) const {
  DAKKU_CHECK(ray.d != Vector3f(0, 0, 0), "ray direction is (0, 0, 0)");
  return aggregate->intersectP(ray);
}

Bounds3f Scene::worldBound() const { return aggregate->worldBound(); }
DAKKU_END
