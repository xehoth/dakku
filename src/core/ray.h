#ifndef DAKKU_CORE_RAY_H_
#define DAKKU_CORE_RAY_H_
#include <core/vector.h>

#include <utility>

namespace dakku {

/**
 * @brief ray
 *
 */
class Ray {
 public:
  Ray() = default;
  explicit Ray(Point3f o, Vector3f d, float tMax = INF)
      : o(std::move(o)), d(std::move(d)), tMax(tMax) {}

  /**
   * @brief get the point on the ray at $t$
   *
   */
  Point3f operator()(float t) const { return o + d * t; }

  /**
   * @brief check whether the ray has nans
   *
   */
  [[nodiscard]] bool hasNaNs() const {
    return o.hasNaNs() || d.hasNaNs() || isNaN(tMax);
  }

  /// ray origin
  Point3f o;
  /// ray direction (note: it may not be normalized)
  Vector3f d;
  mutable float tMax{INF};
};

/**
 * @brief differential ray
 *
 */
class RayDifferential : public Ray {
 public:
  RayDifferential() = default;

  explicit RayDifferential(const Point3f &o, const Vector3f &d,
                           float tMax = INF)
      : Ray(o, d, tMax) {}

  /**
   * @brief check whether the ray differentials has nans
   *
   */
  [[nodiscard]] bool hasNaNs() const {
    return Ray::hasNaNs() || (hasDifferentials &&
                              (rxOrigin.hasNaNs() || ryOrigin.hasNaNs() ||
                               rxDirection.hasNaNs() || ryDirection.hasNaNs()));
  }

  /**
   * @brief scale the differential of the ray
   *
   * @param s scale
   *
   * let $c$ be the camera position  
   * $o'' = o + s (o \rightarrow o')$  
   * $d' = c \rightarrow o'$  
   * $d'' = c \rightarrow o'' = (c \rightarrow o) + (o \rightarrow o'') = d + s
   * (d' - d)$
   */
  void scaleDifferentials(float s) {
    rxOrigin = o + (rxOrigin - o) * s;
    ryOrigin = o + (ryOrigin - o) * s;
    rxDirection = d + (rxDirection - d) * s;
    ryDirection = d + (ryDirection - d) * s;
  }

  /// has differentials
  bool hasDifferentials{false};
  /// x sub ray origin (x + 1, y)
  Point3f rxOrigin;
  /// y sub ray origin (x, y + 1)
  Point3f ryOrigin;
  /// x sub ray direction
  Vector3f rxDirection;
  /// y sub ray direction
  Vector3f ryDirection;
};
}  // namespace dakku
#endif