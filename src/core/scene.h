#ifndef DAKKU_SRC_CORE_SCENE_H_
#define DAKKU_SRC_CORE_SCENE_H_
#include <core/object.h>
#include <vector>

DAKKU_BEGIN
class Scene : public Object {
 public:
  DAKKU_DECLARE_OBJECT(Scene);

  explicit Scene(const Primitive *aggregate, const std::vector<Light *> &lights)
      : lights(lights), aggregate(aggregate) {}

  bool intersect(const Ray &ray, SurfaceInteraction &isect) const;
  [[nodiscard]] bool intersectP(const Ray &ray) const;

  std::vector<Light *> lights;

 private:
  const Primitive *aggregate{};
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_SCENE_H_
