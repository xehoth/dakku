//
// Created by xehoth on 2021/10/1.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_GEOMETRY_H_
#define DAKKU_INCLUDE_DAKKU_CORE_GEOMETRY_H_
#include <dakku/core/dakku.h>
#include <glm/glm.hpp>

namespace dakku {

template <typename T>
requires std::is_arithmetic_v<T>
struct TVector2 {
  TVector2() : v(0) {}
  template <typename U>
  requires std::is_arithmetic_v<U>
  explicit TVector2(U xyz) : v(xyz) {}
  explicit TVector2(const glm::vec<2, T> &v) : v(v) {}
  template <typename U>
  requires std::is_arithmetic_v<U>
  explicit TVector2(U x, U y) : v(x, y) {}

  union {
    struct {
      T x, y;
    };
    struct {
      T s, t;
    };
    glm::vec<2, T> v;
  };

  template <typename Vec>
  requires std::derived_from<Vec, TVector2<T>>
  friend Vec normalize(const Vec &vec) { return Vec(glm::normalize(vec.v)); };
};

template <typename T>
requires std::is_arithmetic_v<T>
struct TVector3 {
  TVector3() : v(0) {}
  template <typename U>
  requires std::is_arithmetic_v<U>
  explicit TVector3(U xyz) : v(xyz) {}
  explicit TVector3(const glm::vec<3, T> &v) : v(v) {}
  template <typename U>
  requires std::is_arithmetic_v<U> TVector3(U x, U y, U z) : v(x, y, z) {}

  union {
    struct {
      T x, y, z;
    };
    struct {
      T r, g, b;
    };
    glm::vec<3, T> v;
  };

  template <typename Vec>
  requires std::derived_from<Vec, TVector3<T>>
  friend Vec normalize(const Vec &vec) { return Vec(glm::normalize(vec.v)); };
};

template <typename T>
requires std::is_arithmetic_v<T>
class Point2 : public TVector2<T> {
 public:
  using TVector2<T>::TVector2;
  explicit Point2(const TVector2<T> &vec) : TVector2<T>(vec) {}
};

using Point2i = Point2<int>;
using Point2f = Point2<Float>;

template <typename T>
requires std::is_arithmetic_v<T>
class Vector3 : public TVector3<T> {
 public:
  using TVector3<T>::TVector3;
  explicit Vector3(const TVector3<T> &vec) : TVector3<T>(vec) {}

  Vector3 operator+(const Vector3 &rhs) const {
    return Vector3(this->v + rhs.v);
  }

  Vector3 operator*(T f) const { return Vector3(this->v * f); }
};

using Vector3f = Vector3<Float>;

template <typename T>
requires std::is_arithmetic_v<T>
class Point3 : public TVector3<T> {
 public:
  using TVector3<T>::TVector3;

  Point3 operator+(const Vector3<T> &v) const { return Point3(this->v + v.v); }
};

using Point3f = Point3<Float>;

template <typename T>
requires std::is_arithmetic_v<T>
class Normal3 : public TVector3<T> {
 public:
  using TVector3<T>::TVector3;
};

using Normal3f = Normal3<Float>;

class Ray {
 public:
  Ray() = default;
  explicit Ray(const Point3f &o, const Vector3f &d, Float tMax = INF);
  Point3f operator()(Float t) const;

  Point3f o;
  Vector3f d;
  mutable Float tMax{INF};
};

}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_GEOMETRY_H_
