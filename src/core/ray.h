#ifndef DAKKU_CORE_RAY_H_
#define DAKKU_CORE_RAY_H_
#include <core/vector.h>
#include <core/constants.h>

namespace dakku {
/**
 * @brief ray
 *
 */
class Ray {
 public:
  Ray() = default;
  explicit Ray(Point3f o, Vector3f d, float t_max = INF)
      : o(std::move(o)), d(std::move(d)), tMax(t_max) {}

  /**
   * @brief get the point on the ray at $t$
   *
   */
  Point3f operator()(float t) const { return o + d * t; }

  /**
   * @brief check whether the ray has nans
   *
   */
  [[nodiscard]] bool has_nans() const {
    return o.has_nans() || d.has_nans() || isnan(tMax);
  }

  /// ray origin
  Point3f o;
  /// ray direction (note: it may not be normalized)
  Vector3f d;
  /// ray max time
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
                           float t_max = INF)
      : Ray(o, d, t_max) {}

  /**
   * @brief check whether the ray differentials has nans
   *
   */
  [[nodiscard]] bool has_nans() const {
    return Ray::has_nans() ||
           (has_differentials &&
            (rx_origin.has_nans() || ry_origin.has_nans() ||
             rx_direction.has_nans() || ry_direction.has_nans()));
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
  void scale_differentials(float s) {
    rx_origin = o + (rx_origin - o) * s;
    ry_origin = o + (ry_origin - o) * s;
    rx_direction = d + (rx_direction - d) * s;
    ry_direction = d + (ry_direction - d) * s;
  }

  /// has differentials
  bool has_differentials{false};
  /// x sub ray origin (x + 1, y)
  Point3f rx_origin;
  /// y sub ray origin (x, y + 1)
  Point3f ry_origin;
  /// x sub ray direction
  Vector3f rx_direction;
  /// y sub ray direction
  Vector3f ry_direction;
};
}  // namespace dakku
#endif