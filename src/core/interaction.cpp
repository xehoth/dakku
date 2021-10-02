//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/interaction.h>

namespace dakku {
Interaction::Interaction(const Point3f &p, const Normal3f &n,
                         const Vector3f &wo)
    : p(p), wo(wo), n(n) {}

bool Interaction::isSurfaceInteraction() const { return n != Normal3f(); }

Ray Interaction::spawnRay(const Vector3f &d) const {
  Point3f o = offsetRayOrigin(p, n, d);
  return Ray{o, d, INF};
}

SurfaceInteraction::SurfaceInteraction(const Point3f &p, const Normal3f &n,
                                       const Vector3f &wo)
    : Interaction(p, n, wo) {}
}  // namespace dakku