#ifndef DAKKU_CORE_VECTOR_H_
#define DAKKU_CORE_VECTOR_H_
#include <core/vector_base.h>
#include <core/lua.h>

namespace dakku {

/**
 * @brief vector
 *
 * @tparam T type
 * @tparam S size
 */
template <typename T, size_t S>
class DAKKU_EXPORT_CORE Vector : public VectorBase<T, S, Vector<T, S>> {
 public:
  using VectorBase<T, S, Vector<T, S>>::VectorBase;

  /**
   * @brief negation of the vector
   *
   * @return $-\vec v$
   */
  Vector operator-() const {
    Vector ret = *this;
    for (size_t i = 0; i < S; ++i) ret[i] = -ret[i];
    return ret;
  }

  /**
   * @brief subtraction
   *
   */
  Vector operator-(const Vector &rhs) const {
    Vector ret = *this;
    ret -= rhs;
    return ret;
  }

  /**
   * @brief subtraction
   *
   */
  template <ArithmeticType V>
  Vector operator-(V rhs) const {
    Vector ret = *this;
    ret -= rhs;
    return ret;
  }
};

using Vector2i = Vector<int, 2>;
using Vector2f = Vector<float, 2>;
using Vector3i = Vector<int, 3>;
using Vector3f = Vector<float, 3>;

DAKKU_DECLARE_LUA_OBJECT(Vector2i);
DAKKU_DECLARE_LUA_OBJECT(Vector2f);
DAKKU_DECLARE_LUA_OBJECT(Vector3i);
DAKKU_DECLARE_LUA_OBJECT(Vector3f);

/**
 * @brief point
 *
 * @tparam T type
 * @tparam S size
 */
template <typename T, size_t S>
class DAKKU_EXPORT_CORE Point : public VectorBase<T, S, Point<T, S>> {
 public:
  using VectorBase<T, S, Point<T, S>>::VectorBase;

  /**
   * @brief negation of the point
   *
   * @return $-v$
   */
  Point operator-() const {
    Point ret = *this;
    for (size_t i = 0; i < S; ++i) ret[i] = -ret[i];
    return ret;
  }

  /**
   * @brief subtraction
   *
   */
  template <ArithmeticType V>
  Point operator-(V rhs) const {
    Point ret = *this;
    ret -= rhs;
    return ret;
  }

  /**
   * @brief subtraction
   *
   */
  Vector<T, S> operator-(const Point &rhs) const {
    Point ret = *this;
    ret -= rhs;
    return Vector<T, S>{ret};
  }

  /**
   * @brief subtraction
   *
   */
  Point operator-(const Vector<T, S> &rhs) const {
    Point ret = *this;
    ret -= Point(rhs);
    return ret;
  }
};

using Point3f = Point<float, 3>;
using Point3i = Point<int, 3>;
using Point2f = Point<float, 2>;
using Point2i = Point<int, 2>;
DAKKU_DECLARE_LUA_OBJECT(Point3f);
DAKKU_DECLARE_LUA_OBJECT(Point3i);
DAKKU_DECLARE_LUA_OBJECT(Point2f);
DAKKU_DECLARE_LUA_OBJECT(Point2i);

/**
 * @brief normal
 *
 * @tparam T type
 * @tparam S size
 */
template <typename T, size_t S>
class DAKKU_EXPORT_CORE Normal : public VectorBase<T, S, Normal<T, S>> {
 public:
  using VectorBase<T, S, Normal<T, S>>::VectorBase;

  /**
   * @brief negation of the vector
   *
   * @return $-\vec v$
   */
  Normal operator-() const {
    Normal ret = *this;
    for (size_t i = 0; i < S; ++i) ret[i] = -ret[i];
    return ret;
  }

  /**
   * @brief subtraction
   *
   */
  Normal operator-(const Normal &rhs) const {
    Normal ret = *this;
    ret -= rhs;
    return ret;
  }

  /**
   * @brief subtraction
   *
   */
  template <ArithmeticType V>
  Normal operator-(V rhs) const {
    Normal ret = *this;
    ret -= rhs;
    return ret;
  }
};

using Normal3f = Normal<float, 3>;
DAKKU_DECLARE_LUA_OBJECT(Normal3f);
}  // namespace dakku
#endif