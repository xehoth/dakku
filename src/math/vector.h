#ifndef DAKKU_MATH_VECTOR_H_
#define DAKKU_MATH_VECTOR_H_
#include <math/vector_base.h>

namespace dakku {

template <ArithmeticType T, size_t S>
class Vector : public VectorBase<T, S, Vector<T, S>> {
 public:
  using VectorBase<T, S, Vector<T, S>>::VectorBase;

  friend Vector operator-(const Vector &a, const Vector &b) {
    Vector ret = a;
    ret -= b;
    return ret;
  }
};

template <ArithmeticType T, size_t S>
class Point : public VectorBase<T, S, Point<T, S>> {
 public:
  using VectorBase<T, S, Point<T, S>>::VectorBase;

  friend Vector<T, S> operator-(const Point &a, const Point &b) {
    return Vector<T, S>(a) - Vector<T, S>(b);
  }
};

template <ArithmeticType T, size_t S>
class Normal : public VectorBase<T, S, Normal<T, S>> {
 public:
  using VectorBase<T, S, Normal<T, S>>::VectorBase;
  
   friend Normal operator-(const Normal &a, const Normal &b) {
    Normal ret = a;
    ret -= b;
    return ret;
  }
};

template <ArithmeticType T>
using Vector2 = Vector<T, 2>;
template <ArithmeticType T>
using Vector3 = Vector<T, 3>;
using Vector2f = Vector2<float>;
using Vector3f = Vector3<float>;

template <ArithmeticType T>
using Point2 = Point<T, 2>;
template <ArithmeticType T>
using Point3 = Point<T, 3>;
using Point2f = Point2<float>;
using Point3f = Point3<float>;

template <ArithmeticType T>
using Normal3 = Normal<T, 3>;
using Normal3f = Normal3<float>;
}  // namespace dakku
#endif