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
  requires std::is_convertible_v<U, T>
  explicit TVector2(U xyz) : v(static_cast<T>(xyz)) {}
  explicit TVector2(const glm::vec<2, T> &v) : v(v) {}
  template <typename U>
  requires std::is_arithmetic_v<U> TVector2(U x, U y) : v(x, y) {}

  union {
    struct {
      T x, y;
    };
    struct {
      T s, t;
    };
    glm::vec<2, T> v;
  };
};

template <typename T>
requires std::is_arithmetic_v<T>
struct TVector3 {
  TVector3() : v(0) {}
  template <typename U>
  requires std::is_convertible_v<U, T>
  explicit TVector3(U xyz) : v(static_cast<T>(xyz)) {}
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

  decltype(auto) operator[](int i) const { return v[i]; }
  decltype(auto) operator[](int i) { return v[i]; }

  [[nodiscard]] decltype(auto) length() const { return glm::length(this->v); }
  [[nodiscard]] decltype(auto) lengthSquared() const {
    return x * x + y * y + z * z;
  }
};

template <typename T>
requires std::is_arithmetic_v<T>
class Vector2 : public TVector2<T> {
 public:
  using TVector2<T>::TVector2;
};

using Vector2f = Vector2<Float>;

template <typename T>
requires std::is_arithmetic_v<T>
class Point2 : public TVector2<T> {
 public:
  using TVector2<T>::TVector2;
  explicit Point2(const TVector2<T> &vec) : TVector2<T>(vec) {}

  Point2 operator*(T f) const { return Point2{this->v * f}; }
  Point2 operator-(const Vector2<T> &vec) const {
    return Point2{this->v - vec.v};
  }
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

  Vector3 operator-() const { return Vector3{-this->v}; }
};

using Vector3f = Vector3<Float>;

template <typename T>
requires std::is_arithmetic_v<T>
class Point3 : public TVector3<T> {
 public:
  using TVector3<T>::TVector3;

  Point3 operator+(const Vector3<T> &v) const { return Point3(this->v + v.v); }
  Point3 operator*(T f) const { return Point3{this->v * f}; }
  Point3 operator+(const Point3 &p) const { return Point3{this->v + p.v}; }
  Vector3<T> operator-(const Point3 &p) const {
    return Vector3<T>{this->v - p.v};
  }
};

using Point3f = Point3<Float>;

template <typename T>
requires std::is_arithmetic_v<T>
class Normal3 : public TVector3<T> {
 public:
  using TVector3<T>::TVector3;

  explicit Normal3(const TVector3<T> &vec) : TVector3<T>(vec) {}

  bool operator!=(const Normal3 &rhs) const { return this->v != rhs.v; }

  Normal3 operator-() const { return Normal3{-this->v}; }

  Normal3 operator*(T f) const { return Normal3{this->v * f}; }

  Normal3 operator+(const Normal3 &rhs) const {
    return Normal3{this->v + rhs.v};
  }
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

template <typename T>
requires std::is_arithmetic_v<T>
decltype(auto) radians(T x) { return glm::radians(x); }

Point3f offsetRayOrigin(const Point3f &p, const Normal3f &n, const Vector3f &w);

template <typename T>
requires std::is_arithmetic_v<T>
inline T dot(const TVector3<T> &a, const TVector3<T> &b) {
  return glm::dot(a.v, b.v);
}

template <typename T>
requires std::is_arithmetic_v<T>
inline T absDot(const TVector3<T> &a, const TVector3<T> &b) {
  return std::abs(dot(a, b));
}

template <typename T>
requires std::is_arithmetic_v<T>
inline Vector3<T> cross(const Vector3<T> &a, const Vector3<T> &b) {
  return Vector3<T>{glm::cross(a.v, b.v)};
}

template <typename T>
requires std::is_arithmetic_v<T>
inline Normal3<T> faceforward(const Normal3<T> &n, const Normal3<T> &n2) {
  return dot(n, n2) < 0 ? -n : n;
}

template <typename T>
requires std::is_arithmetic_v<T>
inline Vector3<T> normalize(const Vector3<T> &vec) {
  return Vector3<T>{glm::normalize(vec.v)};
}

template <typename T>
requires std::is_arithmetic_v<T>
inline Normal3<T> normalize(const Normal3<T> &n) {
  return Normal3<T>{glm::normalize(n.v)};
}
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_GEOMETRY_H_
