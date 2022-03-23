#ifndef DAKKU_SRC_CORE_INTERACTION_H_
#define DAKKU_SRC_CORE_INTERACTION_H_
#include <core/fwd.h>
#include <core/vector.h>

DAKKU_BEGIN
struct Interaction {
  explicit Interaction() = default;
  explicit Interaction(const Point3f &p, const Vector3f &wo) : p(p), wo(wo) {}
  explicit Interaction(const Point3f &p, const Vector3f &wo, const Normal3f &n)
      : p(p), wo(wo), n(n) {}

  Point3f p;
  Vector3f wo;
  Normal3f n;
};

class SurfaceInteraction : public Interaction {
 public:
  explicit SurfaceInteraction() = default;
  explicit SurfaceInteraction(const Point3f &p, const Point2f &uv,
                              const Vector3f &wo, const Normal3f &n,
                              const Shape *shape)
      : Interaction(p, wo, n), uv(uv), shape(shape) {}
  void setShadingGeometry(const Normal3f &n, bool orientationIsAuthoritative);

  Point2f uv;
  struct {
    Normal3f n;
  } shading;
  const Shape *shape{};
  const Primitive *primitive{};
  BSDF *bsdf{};
};
DAKKU_END

#endif  // DAKKU_SRC_CORE_INTERACTION_H_
