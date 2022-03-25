#ifndef DAKKU_SRC_CORE_INTERACTION_H_
#define DAKKU_SRC_CORE_INTERACTION_H_
#include <core/fwd.h>
#include <core/vector.h>
#include <core/ray.h>

DAKKU_BEGIN
struct Interaction {
  explicit Interaction() = default;
  explicit Interaction(const Point3f &p, const Vector3f &wo) : p(p), wo(wo) {}
  explicit Interaction(const Point3f &p, const Vector3f &wo, const Normal3f &n)
      : p(p), wo(wo), n(n) {}

  [[nodiscard]] Ray spawnRay(const Vector3f &d) const {
    Point3f o = offsetRayOrigin(p, n, d);
    return Ray(o, d, INF);
  }
  [[nodiscard]] Ray spawnRayTo(const Point3f &p2) const {
    Point3f origin = offsetRayOrigin(p, n, p2 - p);
    Vector3f d = p2 - p;
    return Ray(origin, d, 1 - SHADOW_EPS);
  }
  [[nodiscard]] Ray spawnRayTo(const Interaction &it) const {
    Point3f origin = offsetRayOrigin(p, n, it.p - p);
    Point3f target = offsetRayOrigin(it.p, it.n, origin - it.p);
    Vector3f d = target - origin;
    return Ray(origin, d, 1 - SHADOW_EPS);
  }

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
  [[nodiscard]] Spectrum emit(const Vector3f &w) const;
  void computeScatteringFunctions(const Ray &ray, MemoryArena &arena);

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
