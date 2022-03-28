#ifndef DAKKU_SRC_CORE_VECTOR_H_
#define DAKKU_SRC_CORE_VECTOR_H_
#include <core/vector_base.h>

DAKKU_BEGIN

template <ArithmeticType T, size_t size>
class Vector : public VectorBase<T, size, Vector<T, size>> {
 public:
  DAKKU_DECLARE_VECTOR_DERIVED(Vector, T, size);

  using super::operator=;
  using super::operator+;
  using super::operator+=;
  using super::operator-;
  using super::operator-=;
};

template <ArithmeticType T, size_t size>
class Normal : public VectorBase<T, size, Normal<T, size>> {
 public:
  DAKKU_DECLARE_VECTOR_DERIVED(Normal, T, size);

  using super::operator=;
  using super::operator-;
  using super::operator+;
};

template <ArithmeticType T, size_t size>
class Point : public VectorBase<T, size, Point<T, size>> {
 public:
  DAKKU_DECLARE_VECTOR_DERIVED(Point, T, size);

  using super::operator+;

  Vector<T, size> operator-(const Point<T, size> &rhs) const {
    return Vector<T, size>(*this) - Vector<T, size>(rhs);
  }
  Point operator+(const Vector<T, size> &rhs) const {
    return static_cast<Point>(super::operator+(super(rhs)));
  }
  Point operator-(const Vector<T, size> &rhs) const {
    return static_cast<Point>(super::operator-(super(rhs)));
  }
};

template <typename T>
inline Normal3<T> faceforward(const Normal3<T> &n, const Vector3<T> &v) {
  return (n.dot(v) < 0) ? -n : n;
}

template <typename T>
inline Normal3<T> faceforward(const Normal3<T> &n, const Normal3<T> &n2) {
  return (n.dot(n2) < 0.f) ? -n : n;
}

template <typename T>
inline Vector3<T> faceforward(const Vector3<T> &v, const Vector3<T> &v2) {
  return (v.dot(v2) < 0.f) ? -v : v;
}

template <typename T>
inline Vector3<T> faceforward(const Vector3<T> &v, const Normal3<T> &n2) {
  return (v.dot(n2) < 0.f) ? -v : v;
}

inline Vector3f sphericalDirection(Float sinTheta, Float cosTheta, Float phi) {
  return Vector3f(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
}

/**
 * construct a local coordinate system according to v1
 */
template <ArithmeticType T>
inline void coordinateSystem(const Vector3<T> &v1, Vector3<T> &v2,
                             Vector3<T> &v3) {
  // choose larger one to get better precision
  if (std::abs(v1.x()) > std::abs(v1.y())) {
    // take v2 as (-z, 0, x), (-z, 0, x) \cdot (x, y, z) = 0
    v2 = Vector3<T>(-v1.z(), 0, v1.x()) /
         std::sqrt(v1.x() * v1.x() + v1.z() * v1.z());
  } else {
    // similar
    v2 = Vector3<T>(0, v1.z(), -v1.y()) /
         std::sqrt(v1.y() * v1.y() + v1.z() * v1.z());
  }
  v3 = v1.cross(v2);
}

DAKKU_END

#endif  // DAKKU_SRC_CORE_VECTOR_H_
