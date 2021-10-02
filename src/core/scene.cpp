//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/scene.h>
#include <dakku/core/primitive.h>

namespace dakku {

Scene::Scene(std::shared_ptr<Primitive> aggregate,
             const std::vector<std::shared_ptr<Light>> &lights)
    : lights(lights), aggregate(aggregate) {}

bool Scene::intersect(const Ray &ray, SurfaceInteraction &isect) const {
  return this->aggregate->intersect(ray, isect);
}

bool Scene::occluded(const Ray &ray) const {
  return this->aggregate->occluded(ray);
}
}  // namespace dakku