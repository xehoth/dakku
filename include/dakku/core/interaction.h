//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_INTERACTION_H_
#define DAKKU_INCLUDE_DAKKU_CORE_INTERACTION_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>
#include <memory>

namespace dakku {

struct Interaction {
  Interaction() = default;
  Interaction(const Point3f &p, const Normal3f &n, const Vector3f &wo);

  bool isSurfaceInteraction() const;
  Ray spawnRay(const Vector3f &d) const;

  Point3f p;
  Vector3f wo;
  Normal3f n;
};

class SurfaceInteraction : public Interaction {
 public:
  SurfaceInteraction() = default;
  SurfaceInteraction(const Point3f &p, const Normal3f &n, const Vector3f &wo);

  std::shared_ptr<const Primitive> primitive{nullptr};
};
}
#endif  // DAKKU_INCLUDE_DAKKU_CORE_INTERACTION_H_
