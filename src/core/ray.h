#pragma once

#include <dakku.h>
#include <spdlog/fmt/ostr.h>
#include <core/geometry.h>

namespace dakku::core {

class Ray {
 public:
  Ray() : t_max(INF) {}
  explicit Ray(const Point3f &o, const Vector3f &d, float t_max = INF)
      : o(o), d(d.normalized()), t_max(t_max) {}

  Point3f operator()(float t) const { return this->at(t); }
  Point3f at(float t) const { return o + d * t; }
  [[nodiscard]] bool has_nans() const {
    return o.has_nans() || d.has_nans() || isnan(t_max);
  }
  Point3f o;
  Vector3f d;
  mutable float t_max;

  template <typename OStream>
  friend OStream &operator<<(OStream &os, const Ray &r) {
    return os << "{o: " << r.o << ", d: " << r.d << ", t_max: " << r.t_max
              << "}";
  }
};

class RayDifferential : public Ray {
 public:
  RayDifferential() = default;
  explicit RayDifferential(const Point3f &o, const Vector3f &d,
                           float t_max = INF)
      : Ray(o, d, t_max) {}
  explicit RayDifferential(const Ray &ray) : Ray(ray) {}

  [[nodiscard]] bool has_nans() const {
    return Ray::has_nans() ||
           (has_differentials &&
            (rx_origin.has_nans() || ry_origin.has_nans() ||
             rx_direction.has_nans() || ry_direction.has_nans()));
  }
  void scale_differentials(float s) {
    rx_origin = o + (rx_origin - o) * s;
    ry_origin = o + (ry_origin - o) * s;
    rx_direction = d + (rx_direction - d) * s;
    ry_direction = d + (ry_direction - d) * s;
  }

  bool has_differentials{};
  // auxiliary rays (x + 1, y) and (x, y + 1)
  Point3f rx_origin, ry_origin;
  Vector3f rx_direction, ry_direction;

  template <typename OStream>
  friend OStream &operator<<(OStream &os, const RayDifferential &r) {
    return os << "{o: " << r.o << ", d: " << r.d << ", t_max: " << r.t_max
              << ", xo: " << r.rx_origin << ", xd: " << r.rx_direction
              << ", yo: " << r.ry_origin << ", yd: " << r.ry_direction << "}";
  }
};
}  // namespace dakku::core