#ifndef DAKKU_CORE_INTERACTION_H_
#define DAKKU_CORE_INTERACTION_H_
#include <core/vector.h>
#include <core/ray.h>
#include <utility>

namespace dakku {

/**
 * @brief interaction
 *
 */
struct Interaction {
  Interaction() = default;
  Interaction(Point3f p, const Vector3f &wo, Normal3f n)
      : p(std::move(p)), wo(wo.normalized()), n(std::move(n)) {}

  /**
   * @brief check whether the interaction is surface interaction
   * surface interaction contains non-zero normal $\vec n$
   *
   */
  [[nodiscard]] bool isSurfaceInteraction() const { return n != Normal3f{}; }

  /**
   * @brief check whether the interaction is medium interaction
   *
   */
  [[nodiscard]] bool isMediumInteraction() const {
    return !isSurfaceInteraction();
  }

  /**
   * @brief spawn a ray to the given direction
   *
   * @param d the direction
   * @return the ray toward `d` after origin offset
   */
  [[nodiscard]] Ray spawnRay(const Vector3f &d) const {
    Point3f o = offsetRayOrigin(p, n, d);
    return Ray(o, d, INF);
  }

  /**
   * @brief spawn ray $p \rightarrow q$
   *
   * @param q the target point
   */
  [[nodiscard]] Ray spawnRayTo(const Point3f &q) const {
    Point3f origin = offsetRayOrigin(p, n, q - p);
    Vector3f d = q - p;
    return Ray(origin, d, 1 - SHADOW_EPS);
  }

  /**
   * @brief spawn ray $p \rightarrow it.p$
   *
   * @param it the given target interaction
   */
  [[nodiscard]] Ray spawnRayTo(const Interaction &it) const {
    Point3f origin = offsetRayOrigin(p, n, it.p - p);
    Point3f target = offsetRayOrigin(it.p, it.n, origin - it.p);
    Vector3f d = target - origin;
    return Ray(origin, d, 1 - SHADOW_EPS);
  }

  /// interaction point
  Point3f p;
  /// outgoing direction
  Vector3f wo;
  /// normal
  Normal3f n;
};

/**
 * @brief surface interaction
 *
 */
class SurfaceInteraction : public Interaction {
 public:
  Point2f uv;
  struct {
    Normal3f n;  // shading normal (interpolated)
    Vector3f dpdu, dpdv;
    Normal3f dndu, dndv;
  } shading;
  mutable float dudx = 0, dvdx = 0, dudy = 0, dvdy = 0;
};
}  // namespace dakku
#endif