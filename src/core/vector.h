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

template <ArithmeticType T>
using Vector2 = Vector<T, 2>;
template <ArithmeticType T>
using Vector3 = Vector<T, 3>;
template <ArithmeticType T>
using Point2 = Point<T, 2>;
template <ArithmeticType T>
using Point3 = Point<T, 3>;

using Vector2f = Vector2<Float>;
using Point2i = Point2<int>;
using Point2f = Point2<Float>;
DAKKU_END

#endif  // DAKKU_SRC_CORE_VECTOR_H_
