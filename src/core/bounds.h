#pragma once

#include <dakku.h>
#include <core/geometry.h>

namespace dakku::core {

template <ArithmeticType T>
class Bounds2 {
 public:
  Bounds2() {
    T min = std::numeric_limits<T>::lowest();
    T max = std::numeric_limits<T>::max();
    p_min = Point2<T>(min, min);
    p_max = Point2<T>(max, max);
  }

  explicit Bounds2(const Point2<T> &p) : p_min(p), p_max(p) {}
  explicit Bounds2(const Point2<T> &p1, const Point2<T> &p2)
      : p_min(p1.cwiseMin(p2)), p_max(p1.cwiseMax(p2)) {}

  template <ArithmeticType U>
  requires std::is_convertible_v<T, U>
  explicit operator Bounds2<U>() const {
    return Bounds2<U>(Point2<U>(p_min), Point2<U>(p_max));
  }

  Vector2<T> diagonal() const { return p_max - p_min; }

  T area() const {
    auto d = diagonal();
    return d.x() * d.y();
  }

  [[nodiscard]] int maximum_extent() const {
    auto d = diagonal();
    return static_cast<int>(d.x() <= d.y());
  }

  const Point2<T> &operator[](int i) const {
    DAKKU_CHECK(i == 0 || i == 1, "Invalid index in bounds");
    return i == 0 ? p_min : p_max;
  }

  Point2<T> &operator[](int i) {
    return const_cast<Point2<T> &>(static_cast<const Bounds2<T> &>(*this)[i]);
  }

  auto operator<=>(const Bounds2<T> &rhs) const = default;

  Point2<T> lerp(const Point2f &t) const {
    return Point2<T>(dakku::lerp(t.x(), p_min.x(), p_max.x()),
                     dakku::lerp(t.y(), p_min.y(), p_max.y()));
  }

  Vector2<T> offset(const Point2<T> &p) const {
    Vector2<T> o = p - p_min;
    if (p_max.x() > p_min.x()) o.x() /= p_max.x() - p_min.x();
    if (p_max.y() > p_min.y()) p.y() /= p_max.y() - p_min.y();
    return o;
  }

  Point2<T> p_min, p_max;

  template <typename OStream>
  friend OStream &operator<<(OStream &os, const Bounds2<T> &b) {
    return os << "{p_min: " << b.p_min << ", p_max: " << b.p_max << "}";
  }
};

template <ArithmeticType T>
class Bounds3 {
 public:
  Bounds3() {
    T min = std::numeric_limits<T>::lowest();
    T max = std::numeric_limits<T>::max();
    p_min = Point3<T>(min, min, min);
    p_max = Point3<T>(max, max, max);
  }
  explicit Bounds3(const Point3<T> &p) : p_min(p), p_max(p) {}
  explicit Bounds3(const Point3<T> &p1, const Point3<T> &p2)
      : p_min(p1.cwiseMin(p2)), p_max(p1.cwiseMax(p2)) {}
  const Point3<T> &operator[](int i) const {
    DAKKU_CHECK(i == 0 || i == 1, "Invalid index in bounds");
    return i == 0 ? p_min : p_max;
  }

  Point3<T> &operator[](int i) {
    return const_cast<Point3<T> &>(static_cast<const Bounds3<T> &>(*this)[i]);
  }

  auto operator<=>(const Bounds3<T> &rhs) const = default;

  // returns the coordinates of one of the eight corners of the bounding box
  Point3<T> corner(int c) const {
    DAKKU_CHECK(0 <= c && c <= 7, "Invalid corner index");
    return Point3<T>((*this)[c & 1].x(), (*this)[(c & 2) ? 1 : 0].y(),
                     (*this)[(c & 4) ? 1 : 0].z());
  }

  Vector3<T> diagonal() const { return p_max - p_min; }

  T surface_area() const {
    auto d = diagonal();
    return 2 * (d.x() * d.y() + d.x() * d.z() + d.y() * d.z());
  }

  T volume() const {
    auto d = diagonal();
    return d.x() * d.y() * d.z();
  }

  [[nodiscard]] int maximum_extent() const {
    auto d = diagonal();
    if (d.x() > d.y() && d.x() > d.z())
      return 0;
    else if (d.y() > d.z())
      return 1;
    return 2;
  }

  Point3<T> lerp(const Point3f &t) const {
    return Point3<T>(dakku::lerp(t.x(), p_min.x(), p_max.x()),
                     dakku::lerp(t.y(), p_min.y(), p_max.y()),
                     dakku::lerp(t.z(), p_min.z(), p_max.z()));
  }

  Vector3<T> offset(const Point3<T> &p) const {
    Vector3<T> o = p - p_min;
    if (p_max.x() > p_min.x()) o.x /= p_max.x() - p_min.x();
    if (p_max.y() > p_min.y()) o.y /= p_max.y() - p_min.y();
    if (p_max.z() > p_min.z()) o.z /= p_max.z() - p_min.z();
    return o;
  }

  template <ArithmeticType U>
  requires std::is_convertible_v<T, U>
  explicit operator Bounds3<U>() const {
    return Bounds3<U>(Point3<U>(p_min), Point3<U>(p_max));
  }

  Point3<T> p_min, p_max;

  template <typename OStream>
  friend OStream &operator<<(OStream &os, const Bounds3<T> &b) {
    return os << "{p_min: " << b.p_min << ", p_max: " << b.p_max << "}";
  }
};

using Bounds2f = Bounds2<float>;
using Bounds2i = Bounds2<int>;
using Bounds3f = Bounds3<float>;
using Bounds3i = Bounds3<int>;


}  // namespace dakku::core