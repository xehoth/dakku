//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_SCENE_H_
#define DAKKU_INCLUDE_DAKKU_CORE_SCENE_H_
#include <dakku/core/dakku.h>
#include <memory>
#include <vector>

namespace dakku {

class Scene {
 public:
  Scene(std::shared_ptr<Primitive> aggregate,
        const std::vector<std::shared_ptr<Light>> &lights);

  bool intersect(const Ray &ray, SurfaceInteraction &isect) const;
  [[nodiscard]] bool occluded(const Ray &ray) const;

  std::vector<std::shared_ptr<Light>> lights;
 private:
  std::shared_ptr<Primitive> aggregate{nullptr};
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_SCENE_H_
