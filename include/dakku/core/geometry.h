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
struct IVector3 {
  IVector3() : v(0) {}
  template <typename U>
  requires std::is_arithmetic_v<U>
  explicit IVector3(U xyz) : v(xyz) {}
  explicit IVector3(const glm::vec<3, T> &v) : v(v) {}
  template <typename U>
  requires std::is_arithmetic_v<U> IVector3(U x, U y, U z) : v(x, y, z) {}

  union {
    struct {
      T x, y, z;
    };
    glm::vec<3, T> v;
  };

  template <typename Vec>
  requires std::derived_from<Vec, IVector3<T>>
  friend Vec normalize(const Vec &vec) { return Vec(glm::normalize(vec.v)); };
};

template <typename T>
requires std::is_arithmetic_v<T>
class Vector3 : public IVector3<T> {
 public:
  using IVector3<T>::IVector3;
  explicit Vector3(const IVector3<T> &vec) : IVector3<T>(vec) {}

  Vector3 operator+(const Vector3 &rhs) const {
    return Vector3(this->v + rhs.v);
  }

  Vector3 operator*(T f) const { return Vector3(this->v * f); }
};

using Vector3f = Vector3<Float>;

template <typename T>
requires std::is_arithmetic_v<T>
class Point3 : public IVector3<T> {
 public:
  using IVector3<T>::IVector3;

  Point3 operator+(const Vector3<T> &v) const { return Point3(this->v + v.v); }
};

using Point3f = Point3<Float>;

template <typename T>
requires std::is_arithmetic_v<T>
class Normal3 : public IVector3<T> {
 public:
  using IVector3<T>::IVector3;
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
