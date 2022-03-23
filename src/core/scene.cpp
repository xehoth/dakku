#include <core/scene.h>
#include <core/interaction.h>
#include <core/ray.h>
#include <core/logger.h>
#include <core/primitive.h>

DAKKU_BEGIN

bool Scene::intersect(const Ray &ray, SurfaceInteraction &isect) const {
  DAKKU_CHECK(ray.d != Vector3f(0, 0, 0), "ray direction is (0, 0, 0)");
  return aggregate->intersect(ray, isect);
}

bool Scene::intersectP(const Ray &ray) const {
  DAKKU_CHECK(ray.d != Vector3f(0, 0, 0), "ray direction is (0, 0, 0)");
  return aggregate->intersectP(ray);
}
DAKKU_END
