//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/interaction.h>
#include <dakku/core/primitive.h>
#include <dakku/core/light.h>
#include <dakku/core/spectrum.h>

namespace dakku {
Interaction::Interaction(const Point3f &p, const Normal3f &n,
                         const Vector3f &wo)
    : p(p), wo(wo), n(n) {}

bool Interaction::isSurfaceInteraction() const { return n != Normal3f(); }

Ray Interaction::spawnRay(const Vector3f &d) const {
  Point3f o = offsetRayOrigin(p, n, d);
  return Ray{o, d, INF};
}
Ray Interaction::spawnRayTo(const Interaction &it) const {
  Point3f origin = offsetRayOrigin(p, n, it.p - p);
  Point3f target = offsetRayOrigin(it.p, it.n, origin - it.p);
  Vector3f d = target - origin;
  return Ray{origin, d, 1 - SHADOW_EPS};
}

SurfaceInteraction::SurfaceInteraction(const Point3f &p, const Normal3f &n,
                                       const Vector3f &wo)
    : Interaction(p, n, wo) {}

RGBSpectrum SurfaceInteraction::emit(const Vector3f &w) const {
  std::shared_ptr<const AreaLight> area = primitive->getAreaLight();
  return area ? area->emit(*this, w) : RGBSpectrum(0);
}

void SurfaceInteraction::computeScatteringFunctions(const Ray &ray) {
  primitive->computeScatteringFunctions(*this);
}
}  // namespace dakku