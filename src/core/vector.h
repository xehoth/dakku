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
 * @tparam D derived
 */
template <typename T, size_t S, typename D>
class DAKKU_EXPORT_CORE Vector : public VectorBase<T, S, D> {
 public:
  using VectorBase<T, S, D>::VectorBase;

  /**
   * @brief negation of the vector
   *
   * @return $-\vec v$
   */
  D operator-() const {
    D ret = this->derived();
    for (size_t i = 0; i < S; ++i) ret[i] = -ret[i];
    return ret;
  }

  /**
   * @brief subtraction
   *
   */
  D operator-(const D &rhs) const {
    D ret = this->derived();
    ret -= rhs;
    return ret;
  }

  /**
   * @brief subtraction
   *
   */
  template <ArithmeticType V>
  D operator-(V rhs) const {
    D ret = this->derived();
    ret -= rhs;
    return ret;
  }
};

#define DAKKU_DECLARE_VECTOR_TYPE(name, base, T, S)        \
  class DAKKU_EXPORT_CORE name : public base<T, S, name> { \
   public:                                                 \
    using base<T, S, name>::base;                          \
                                                           \
   private:                                                \
    DAKKU_DECLARE_LUA_OBJECT(name);                        \
  }

DAKKU_DECLARE_VECTOR_TYPE(Vector2i, Vector, int, 2);
DAKKU_DECLARE_VECTOR_TYPE(Vector2f, Vector, float, 2);
DAKKU_DECLARE_VECTOR_TYPE(Vector3i, Vector, int, 3);
DAKKU_DECLARE_VECTOR_TYPE(Vector3f, Vector, float, 3);

/**
 * @brief point
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived
 */
template <typename T, size_t S, typename D>
class DAKKU_EXPORT_CORE Point : public VectorBase<T, S, D> {
 public:
  using VectorBase<T, S, D>::VectorBase;
};

#define DAKKU_DECLARE_POINT_TYPE(name, base, T, S, vector_type) \
  class DAKKU_EXPORT_CORE name : public base<T, S, name> {      \
   public:                                                      \
    using base<T, S, name>::base;                               \
    name operator-() const {                                    \
      name ret = *this;                                         \
      for (size_t i = 0; i < size(); ++i) ret[i] = -ret[i];     \
      return ret;                                               \
    }                                                           \
    template <ArithmeticType V>                                 \
    name operator-(V rhs) const {                               \
      name ret = *this;                                         \
      ret -= rhs;                                               \
      return ret;                                               \
    }                                                           \
    vector_type operator-(const name &rhs) const {              \
      name ret = *this;                                         \
      ret -= rhs;                                               \
      return vector_type{ret};                                  \
    }                                                           \
    name operator-(const vector_type &rhs) const {              \
      name ret = *this;                                         \
      ret -= name(rhs);                                         \
      return ret;                                               \
    }                                                           \
                                                                \
   private:                                                     \
    DAKKU_DECLARE_LUA_OBJECT(name);                             \
  }

DAKKU_DECLARE_POINT_TYPE(Point3f, Point, float, 3, Vector3f);
DAKKU_DECLARE_POINT_TYPE(Point3i, Point, int, 3, Vector3i);
DAKKU_DECLARE_POINT_TYPE(Point2f, Point, float, 2, Vector2f);
DAKKU_DECLARE_POINT_TYPE(Point2i, Point, int, 2, Vector2i);

/**
 * @brief normal
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived
 */
template <typename T, size_t S, typename D>
class DAKKU_EXPORT_CORE Normal : public VectorBase<T, S, D> {
 public:
  using VectorBase<T, S, D>::VectorBase;

  /**
   * @brief negation of the vector
   *
   * @return $-\vec v$
   */
  D operator-() const {
    D ret = this->derived();
    for (size_t i = 0; i < S; ++i) ret[i] = -ret[i];
    return ret;
  }

  /**
   * @brief subtraction
   *
   */
  D operator-(const D &rhs) const {
    D ret = this->derived();
    ret -= rhs;
    return ret;
  }

  /**
   * @brief subtraction
   *
   */
  template <ArithmeticType V>
  D operator-(V rhs) const {
    D ret = this->derived();
    ret -= rhs;
    return ret;
  }
};

DAKKU_DECLARE_VECTOR_TYPE(Normal3f, Normal, float, 3);
}  // namespace dakku
#endif