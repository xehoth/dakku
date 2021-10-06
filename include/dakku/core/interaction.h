//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_INTERACTION_H_
#define DAKKU_INCLUDE_DAKKU_CORE_INTERACTION_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>
#include <dakku/core/reflection.h>
#include <memory>

namespace dakku {

struct Interaction {
  Interaction() = default;
  Interaction(const Point3f &p, const Normal3f &n, const Vector3f &wo);

  [[nodiscard]] bool isSurfaceInteraction() const;
  [[nodiscard]] Ray spawnRay(const Vector3f &d) const;
  [[nodiscard]] Ray spawnRayTo(const Interaction &it) const;
  Point3f p;
  Vector3f wo;
  Normal3f n;
};

class SurfaceInteraction : public Interaction {
 public:
  SurfaceInteraction() = default;
  SurfaceInteraction(const Point3f &p, const Normal3f &n, const Vector3f &wo);
  [[nodiscard]] RGBSpectrum emit(const Vector3f &w) const;
  std::shared_ptr<const Primitive> primitive{nullptr};
  Point2f uv;
  BSDF bsdf;

  void computeScatteringFunctions(const Ray &ray);
};
}
#endif  // DAKKU_INCLUDE_DAKKU_CORE_INTERACTION_H_
