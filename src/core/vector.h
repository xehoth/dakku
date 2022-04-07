#ifndef DAKKU_CORE_VECTOR_H_
#define DAKKU_CORE_VECTOR_H_
#include <core/vector_base.h>

namespace dakku {

/**
 * @brief vector
 *
 * @tparam T data type
 * @tparam S size
 */
template <ArithmeticType T, size_t S>
class Vector : public VectorBase<T, S, Vector<T, S>> {
 public:
  using VectorBase<T, S, Vector<T, S>>::VectorBase;

  /**
   * @brief subtraction between vectors
   *
   */
  friend Vector operator-(const Vector &a, const Vector &b) {
    Vector ret = a;
    ret -= b;
    return ret;
  }
};

/**
 * @brief point
 *
 * @tparam T data type
 * @tparam S size
 */
template <ArithmeticType T, size_t S>
class Point : public VectorBase<T, S, Point<T, S>> {
 public:
  using VectorBase<T, S, Point<T, S>>::VectorBase;

  /**
   * @brief subtraction between two points yields vectors
   *
   */
  friend Vector<T, S> operator-(const Point &a, const Point &b) {
    return Vector<T, S>(a) - Vector<T, S>(b);
  }

  /**
   * @brief point - vector => point
   *
   */
  friend Point operator-(const Point &a, const Vector<T, S> &b) {
    return a - Point(b);
  }

  /**
   * @brief point + vector => point
   *
   */
  friend Point operator+(const Point &a, const Vector<T, S> &b) {
    return a + Point(b);
  }
};

/**
 * @brief normal
 *
 * @tparam T data type
 * @tparam S size
 */
template <ArithmeticType T, size_t S>
class Normal : public VectorBase<T, S, Normal<T, S>> {
 public:
  using VectorBase<T, S, Normal<T, S>>::VectorBase;

  /**
   * @brief subtraction between two normals
   *
   */
  friend Normal operator-(const Normal &a, const Normal &b) {
    Normal ret = a;
    ret -= b;
    return ret;
  }
};

/**
 * @brief 2d vectors
 *
 * @tparam T data type
 */
template <ArithmeticType T>
using Vector2 = Vector<T, 2>;

/**
 * @brief 3d vectors
 *
 * @tparam T data type
 */
template <ArithmeticType T>
using Vector3 = Vector<T, 3>;

/// 2d float vector
using Vector2f = Vector2<float>;
/// 3d float vector
using Vector3f = Vector3<float>;

/**
 * @brief 2d point
 *
 * @tparam T data type
 */
template <ArithmeticType T>
using Point2 = Point<T, 2>;

/**
 * @brief 3d point
 *
 * @tparam T data type
 */
template <ArithmeticType T>
using Point3 = Point<T, 3>;

/// 2d float point
using Point2f = Point2<float>;
/// 2d int point
using Point2i = Point2<int>;
/// 3d float point
using Point3f = Point3<float>;

/**
 * @brief 3d normal
 *
 * @tparam T data type
 */
template <ArithmeticType T>
using Normal3 = Normal<T, 3>;
/// 3d float normal
using Normal3f = Normal3<float>;
}  // namespace dakku
#endif