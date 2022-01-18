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

template <ArithmeticType T>
inline Bounds3<T> union_bounds(const Bounds3<T> &a, const Point3<T> &p) {
  Bounds3<T> ret;
  ret.p_min = min(a.p_min, p);
  ret.p_max = max(a.p_max, p);
  return ret;
}

template <ArithmeticType T>
inline Bounds3<T> union_bounds(const Bounds3<T> &a, const Bounds3<T> &b) {
  Bounds3<T> ret;
  ret.p_min = min(a.p_min, b.p_min);
  ret.p_max = max(a.p_max, b.p_max);
  return ret;
}

template <ArithmeticType T>
inline Bounds3<T> intersect(const Bounds3<T> &a, const Bounds3<T> &b) {
  Bounds3<T> ret;
  ret.p_min = max(a.p_min, b.p_min);
  ret.p_max = min(a.p_max, b.p_max);
  return ret;
}

template <ArithmeticType T>
inline bool overlaps(const Bounds3<T> &a, const Bounds3<T> &b) {
  bool x = (a.p_max.x() >= b.p_min.x()) && (a.p_min.x() <= b.p_max.x());
  bool y = (a.p_max.y() >= b.p_min.y()) && (a.p_min.y() <= b.p_max.y());
  bool z = (a.p_max.z() >= b.p_min.z()) && (a.p_min.z() <= b.p_max.z());
  return (x && y && z);
}

template <ArithmeticType T>
inline bool inside(const Point3<T> &p, const Bounds3<T> &b) {
  return (p.x() >= b.p_min.x() && p.x() <= b.p_max.x() &&
          p.y() >= b.p_min.y() && p.y() <= b.p_max.y() &&
          p.z() >= b.p_min.z() && p.z() <= b.p_max.z());
}

template <ArithmeticType T>
inline bool inside_exclusive(const Point3<T> &p, const Bounds3<T> &b) {
  return (p.x() >= b.p_min.x() && p.x() < b.p_max.x() && p.y() >= b.p_min.y() &&
          p.y() < b.p_max.y() && p.z() >= b.p_min.z() && p.z() < b.p_max.z());
}

template <ArithmeticType T, ArithmeticType U>
requires std::is_same_v<T, std::common_type_t<T, U>>
inline Bounds3<T> expand(const Bounds3<T> &b, U delta) {
  return Bounds3<T>(b.p_min - Vector3<T>(delta, delta, delta),
                    b.p_max + Vector3<T>(delta, delta, delta));
}

// min squared distance from point to box; returns zero if point is inside.
template <ArithmeticType T, ArithmeticType U>
inline float distance_squared(const Point3<T> &p, const Bounds3<U> &b) {
  float dx = std::max<float>(0.0f, b.p_min.x() - p.x(), p.x() - b.p_max.x());
  float dy = std::max<float>(0.0f, b.p_min.y() - p.y(), p.y() - b.p_max.y());
  float dz = std::max<float>(0.0f, b.p_min.z() - p.z(), p.z() - b.p_max.z());
  return dx * dx + dy * dy + dz * dz;
}

template <ArithmeticType T, ArithmeticType U>
inline float distance(const Point3<T> &p, const Bounds3<U> &b) {
  return std::sqrt(distance_squared(p, b));
}

template <ArithmeticType T>
inline Bounds2<T> union_bounds(const Bounds2<T> &b, const Point2<T> &p) {
  Bounds2<T> ret;
  ret.p_min = min(b.p_min, p);
  ret.p_max = max(b.p_max, p);
  return ret;
}

template <ArithmeticType T>
inline Bounds2<T> union_bounds(const Bounds2<T> &a, const Bounds2<T> &b) {
  Bounds2<T> ret;
  ret.p_min = min(a.p_min, b.p_min);
  ret.p_max = max(a.p_max, b.p_max);
  return ret;
}

template <ArithmeticType T>
inline Bounds2<T> intersect(const Bounds2<T> &a, const Bounds2<T> &b) {
  Bounds2<T> ret;
  ret.p_min = max(a.p_min, b.p_min);
  ret.p_max = min(a.p_max, b.p_max);
  return ret;
}

template <ArithmeticType T>
inline bool overlaps(const Bounds2<T> &ba, const Bounds2<T> &bb) {
  bool x = (ba.p_max.x() >= bb.p_min.x()) && (ba.p_min.x() <= bb.p_max.x());
  bool y = (ba.p_max.y() >= bb.p_min.y()) && (ba.p_min.y() <= bb.p_max.y());
  return (x && y);
}

template <ArithmeticType T>
inline bool inside(const Point2<T> &pt, const Bounds2<T> &b) {
  return (pt.x() >= b.p_min.x() && pt.x() <= b.p_max.x() &&
          pt.y() >= b.p_min.y() && pt.y() <= b.p_max.y());
}

template <ArithmeticType T>
inline bool inside_exclusive(const Point2<T> &pt, const Bounds2<T> &b) {
  return (pt.x() >= b.p_min.x() && pt.x() < b.p_max.x() &&
          pt.y() >= b.p_min.y() && pt.y() < b.p_max.y());
}

template <ArithmeticType T, ArithmeticType U>
requires std::is_same_v<T, std::common_type_t<T, U>>
inline Bounds2<T> expand(const Bounds2<T> &b, U delta) {
  return Bounds2<T>(b.p_min - Vector2<T>(delta, delta),
                    b.p_max + Vector2<T>(delta, delta));
}



}  // namespace dakku::core