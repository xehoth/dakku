#pragma once

#include <dakku.h>
#include <core/vector.h>

namespace dakku {

class Ray {
 public:
  Ray() = default;
  explicit Ray(const Point3f &o, const Vector3f &d, Float t_max = INF)
      : o(o), d(d), t_max(t_max) {}

  Point3f at(Float t) const { return o + d * t; }
  Point3f operator()(Float t) const { return at(t); }
  bool has_nans() const { return o.has_nans() || d.has_nans() || isnan(t_max); }

  template <typename OStream>
  friend OStream &operator<<(OStream &os, const Ray &r) {
    return os << "{o: " << r.o << ", d: " << r.d << ", t_max: " << r.t_max
              << "}";
  }

  Point3f o;   // origin
  Vector3f d;  // direction (not normalized)
  mutable Float t_max{INF};
};

class RayDifferential : public Ray {
 public:
  RayDifferential() = default;
  explicit RayDifferential(const Point3f &o, const Vector3f &d, Float t_max)
      : Ray(o, d, t_max) {}
  RayDifferential(const Ray &ray) : Ray(ray) {}
  bool has_nans() const {
    return Ray::has_nans() ||
           (has_differentials &&
            (rx_origin.has_nans() || ry_origin.has_nans() ||
             rx_direction.has_nans() || ry_direction.has_nans()));
  }
  void scale_differentials(Float s) {
    rx_origin = o + (rx_origin - o) * s;
    ry_origin = o + (ry_origin - o) * s;
    rx_direction = d + (rx_direction - d) * s;
    ry_direction = d + (ry_direction - d) * s;
  }
  template <typename OStream>
  friend OStream &operator<<(OStream &os, const RayDifferential &r) {
    return os << "{ray: " << static_cast<const Ray &>(r)
              << ", has_differentials: " << r.has_differentials
              << ", rx_origin: " << r.rx_origin << "ry_origin: " << r.ry_origin
              << ", rx_direction: " << r.rx_direction << ", ry_direction"
              << r.ry_direction << "}";
  }

  bool has_differentials{false};
  Point3f rx_origin, ry_origin;  // (+1, 0), (0, +1)
  Vector3f rx_direction, ry_direction;
};
}  // namespace dakku