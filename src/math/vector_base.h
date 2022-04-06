#ifndef DAKKU_MATH_VECTOR_BASE_H_
#define DAKKU_MATH_VECTOR_BASE_H_
#include <math/fwd.h>
#include <logger/logger.h>
#include <array>
#include <algorithm>
#include <string>
#include <ostream>

namespace dakku {

/**
 * @brief expression template for vector operations
 *
 * @tparam T data type
 * @tparam S size of the vector
 * @tparam Derived final vector type (point/vector/normal)
 * @tparam Expr vector expression
 */
template <ArithmeticType T, size_t S, typename Derived, typename Expr>
struct VectorExpression {
  /**
   * @brief get the i'th value
   *
   * @param i the index
   * @return the value
   */
  decltype(auto) operator[](size_t i) {
    auto ret = static_cast<const VectorExpression &>(*this)[i];
    if constexpr (std::is_lvalue_reference_v<decltype(ret)>)
      return const_cast<T &>(ret);
    return ret;
  }

  /**
   * @brief get the i'th value
   *
   * @param i the index
   * @return the value
   */
  decltype(auto) operator[](size_t i) const {
    return static_cast<const Expr &>(*this).get(i);
  }

  /**
   * @brief Get the derived expression
   *
   * @return the derived expression
   */
  decltype(auto) getExpression() const {
    return static_cast<const Expr &>(*this);
  }

  /**
   * @brief Get the derived expression
   *
   * @return the derived expression
   */
  decltype(auto) getExpression() {
    return const_cast<Expr &>(
        static_cast<const VectorExpression &>(*this).getExpression());
  }
};

/**
 * @brief scalar expression
 *
 * @tparam T type
 * @tparam S size
 * @tparam Derived derived type
 */
template <ArithmeticType T, size_t S, typename Derived>
class VectorScalarExpression
    : public VectorExpression<T, S, Derived,
                              VectorScalarExpression<T, S, Derived>> {
 public:
  explicit VectorScalarExpression(const T &value) : value(value) {}
  decltype(auto) get(size_t) const { return value; }

 private:
  const T &value;
};

/**
 * @brief cast expression
 *
 * @tparam T type
 * @tparam S size
 * @tparam Derived derived type
 * @tparam Expr expression type
 */
template <ArithmeticType T, size_t S, typename Derived, typename Expr>
class VectorCastExpression
    : public VectorExpression<T, S, Derived,
                              VectorCastExpression<T, S, Derived, Expr>> {
 public:
  explicit VectorCastExpression(const Expr &_e) : e(_e) {}
  explicit VectorCastExpression(Expr &&_e) : e(std::move(_e)) {}
  decltype(auto) get(size_t i) const { return static_cast<T>(e[i]); }

 private:
  const Expr &e;
};

/**
 * @brief binary expression
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expression 1
 * @tparam E2 expression 2
 * @tparam Expr derived binary expressions
 */
template <ArithmeticType T, size_t S, typename D, typename E1, typename E2,
          typename Expr>
class BinaryVectorExpression : public VectorExpression<T, S, D, Expr> {
 public:
  explicit BinaryVectorExpression(const E1 &u, const E2 &v) : u(u), v(v) {}

 protected:
  const E1 &u;
  const E2 &v;
};

/**
 * @brief unary expression
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E expression
 * @tparam Expr derived unary expression
 */
template <ArithmeticType T, size_t S, typename D, typename E, typename Expr>
class UnaryVectorExpression : public VectorExpression<T, S, D, Expr> {
 public:
  explicit UnaryVectorExpression(const E &e) : e(e) {}

 protected:
  const E &e;
};

/**
 * @brief vector add expression
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expression 1
 * @tparam E2 expression 2
 */
template <ArithmeticType T, size_t S, typename D, typename E1, typename E2>
class VectorAddExpression
    : public BinaryVectorExpression<T, S, D, E1, E2,
                                    VectorAddExpression<T, S, D, E1, E2>> {
 public:
  using BinaryVectorExpression<
      T, S, D, E1, E2,
      VectorAddExpression<T, S, D, E1, E2>>::BinaryVectorExpression;

  decltype(auto) get(size_t i) const { return this->u[i] + this->v[i]; }
};

/**
 * @brief operator + between vectors
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expr 1 type
 * @tparam E2 expr 2 type
 * @param u expr 1
 * @param v expr 2
 * @return add expr
 */
template <ArithmeticType T, size_t S, typename D, typename E1, typename E2>
inline VectorAddExpression<T, S, D, E1, E2> operator+(
    const VectorExpression<T, S, D, E1> &u,
    const VectorExpression<T, S, D, E2> &v) {
  return VectorAddExpression<T, S, D, E1, E2>{u.getExpression(),
                                              v.getExpression()};
}

/**
 * @brief operator + (vector, scalar)
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expr type
 * @tparam V scalar type
 * @param u expr
 * @param v scalar
 * @return add expr
 */
template <ArithmeticType T, size_t S, typename D, typename E1, ArithmeticType V>
inline VectorAddExpression<
    T, S, D, E1, VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>>
operator+(const VectorExpression<T, S, D, E1> &u, const V &v) {
  return VectorAddExpression<
      T, S, D, E1,
      VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>>{
      u.getExpression(),
      VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>{
          VectorScalarExpression<V, S, D>{v}}};
}

/**
 * @brief operator + (scalar, vector)
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expr type
 * @tparam V scalar type
 * @param v scalar
 * @param u expr
 * @return add expr
 */
template <ArithmeticType T, size_t S, typename D, typename E1, ArithmeticType V>
inline VectorAddExpression<
    T, S, D, VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>, E1>
operator+(const V &v, const VectorExpression<T, S, D, E1> &u) {
  return VectorAddExpression<
      T, S, D, VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>,
      E1>{VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>{
              VectorScalarExpression<V, S, D>{v}},
          u.getExpression()};
}

/**
 * @brief vector sub expression
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expression 1
 * @tparam E2 expression 2
 */
template <ArithmeticType T, size_t S, typename D, typename E1, typename E2>
class VectorSubExpression
    : public BinaryVectorExpression<T, S, D, E1, E2,
                                    VectorSubExpression<T, S, D, E1, E2>> {
 public:
  using BinaryVectorExpression<
      T, S, D, E1, E2,
      VectorSubExpression<T, S, D, E1, E2>>::BinaryVectorExpression;

  decltype(auto) get(size_t i) const { return this->u[i] - this->v[i]; }
};

/**
 * @brief operator - between vectors
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expr 1 type
 * @tparam E2 expr 2 type
 * @param u expr 1
 * @param v expr 2
 * @return add expr
 */
template <ArithmeticType T, size_t S, typename D, typename E1, typename E2>
inline VectorSubExpression<T, S, D, E1, E2> operator-(
    const VectorExpression<T, S, D, E1> &u,
    const VectorExpression<T, S, D, E2> &v) {
  return VectorSubExpression<T, S, D, E1, E2>{u.getExpression(),
                                              v.getExpression()};
}

template <ArithmeticType T, size_t S, typename D, typename E1, ArithmeticType V>
inline VectorSubExpression<
    T, S, D, E1, VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>>
operator-(const VectorExpression<T, S, D, E1> &u, const V &v) {
  return VectorSubExpression<
      T, S, D, E1,
      VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>>{
      u.getExpression(),
      VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>{
          VectorScalarExpression<V, S, D>{v}}};
}

template <ArithmeticType T, size_t S, typename D, typename E1, ArithmeticType V>
inline VectorSubExpression<
    T, S, D, VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>, E1>
operator-(const V &v, const VectorExpression<T, S, D, E1> &u) {
  return VectorSubExpression<
      T, S, D, VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>,
      E1>{VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>{
              VectorScalarExpression<V, S, D>{v}},
          u.getExpression()};
}

/**
 * @brief vector mul expression
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expression 1
 * @tparam E2 expression 2
 */
template <ArithmeticType T, size_t S, typename D, typename E1, typename E2>
class VectorMulExpression
    : public BinaryVectorExpression<T, S, D, E1, E2,
                                    VectorMulExpression<T, S, D, E1, E2>> {
 public:
  using BinaryVectorExpression<
      T, S, D, E1, E2,
      VectorMulExpression<T, S, D, E1, E2>>::BinaryVectorExpression;

  decltype(auto) get(size_t i) const { return this->u[i] * this->v[i]; }
};

/**
 * @brief operator * between vectors
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expr 1 type
 * @tparam E2 expr 2 type
 * @param u expr 1
 * @param v expr 2
 * @return add expr
 */
template <ArithmeticType T, size_t S, typename D, typename E1, typename E2>
inline VectorMulExpression<T, S, D, E1, E2> operator*(
    const VectorExpression<T, S, D, E1> &u,
    const VectorExpression<T, S, D, E2> &v) {
  return VectorMulExpression<T, S, D, E1, E2>{u.getExpression(),
                                              v.getExpression()};
}

template <ArithmeticType T, size_t S, typename D, typename E1, ArithmeticType V>
inline VectorMulExpression<
    T, S, D, E1, VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>>
operator*(const VectorExpression<T, S, D, E1> &u, const V &v) {
  return VectorMulExpression<
      T, S, D, E1,
      VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>>{
      u.getExpression(),
      VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>{
          VectorScalarExpression<V, S, D>{v}}};
}

template <ArithmeticType T, size_t S, typename D, typename E1, ArithmeticType V>
inline VectorMulExpression<
    T, S, D, VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>, E1>
operator*(const V &v, const VectorExpression<T, S, D, E1> &u) {
  return VectorMulExpression<
      T, S, D, VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>,
      E1>{VectorCastExpression<T, S, D, VectorScalarExpression<V, S, D>>{
              VectorScalarExpression<V, S, D>{v}},
          u.getExpression()};
}

/**
 * @brief vector div expression
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expression 1
 * @tparam E2 expression 2
 */
template <ArithmeticType T, size_t S, typename D, typename E1, typename E2>
class VectorDivExpression
    : public BinaryVectorExpression<T, S, D, E1, E2,
                                    VectorDivExpression<T, S, D, E1, E2>> {
 public:
  using BinaryVectorExpression<
      T, S, D, E1, E2,
      VectorDivExpression<T, S, D, E1, E2>>::BinaryVectorExpression;

  decltype(auto) get(size_t i) const { return this->u[i] / this->v[i]; }
};

/**
 * @brief operator / between vectors
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E1 expr 1 type
 * @tparam E2 expr 2 type
 * @param u expr 1
 * @param v expr 2
 * @return add expr
 */
template <ArithmeticType T, size_t S, typename D, typename E1, typename E2>
inline VectorDivExpression<T, S, D, E1, E2> operator/(
    const VectorExpression<T, S, D, E1> &u,
    const VectorExpression<T, S, D, E2> &v) {
  return VectorDivExpression<T, S, D, E1, E2>{u.getExpression(),
                                              v.getExpression()};
}

template <ArithmeticType T, size_t S, typename D, typename E1, ArithmeticType V>
inline decltype(auto) operator/(const VectorExpression<T, S, D, E1> &u,
                                const V &v) {
  return u * static_cast<T>(T{1} / v);
}

/**
 * @brief dot product of two vectors
 *
 * @param a vector `a`
 * @param b vector `b`
 * @return $\vec a \cdot \vec b$
 */
template <ArithmeticType T, size_t S, typename D1, typename E1, typename D2,
          typename E2>
inline T dot(const VectorExpression<T, S, D1, E1> &a,
             const VectorExpression<T, S, D2, E2> &b) {
  T ret = 0;
  for (size_t i = 0; i < S; ++i) ret += a[i] * b[i];
  return ret;
}

/**
 * @brief the squared norm of the vector
 *
 * @param v the vector
 * @return $||\vec v|| ^ 2$
 */
template <ArithmeticType T, size_t S, typename D, typename E>
inline decltype(auto) squaredNorm(const VectorExpression<T, S, D, E> &v) {
  return dot(v, v);
}

/**
 * @brief the norm of the vector
 *
 * @param v the vector
 * @return $||v||$
 */
template <ArithmeticType T, size_t S, typename D, typename E>
inline decltype(auto) norm(const VectorExpression<T, S, D, E> &v) {
  return std::sqrt(squaredNorm(v));
}

/**
 * @brief inverse the vector
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E expr
 */
template <ArithmeticType T, size_t S, typename D, typename E>
class VectorInverseExpression
    : public UnaryVectorExpression<T, S, D, E,
                                   VectorInverseExpression<T, S, D, E>> {
 public:
  using UnaryVectorExpression<
      T, S, D, E, VectorInverseExpression<T, S, D, E>>::UnaryVectorExpression;

  decltype(auto) get(size_t i) const { return T{1} / this->e[i]; }
};

/**
 * @brief inverse the vector
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E expr
 * @param v vector
 * @return the inversion
 */
template <ArithmeticType T, size_t S, typename D, typename E>
inline VectorInverseExpression<T, S, D, E> inverse(
    const VectorExpression<T, S, D, E> &v) {
  return VectorInverseExpression<T, S, D, E>{v};
}

/**
 * @brief negate the vector
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E expr
 */
template <ArithmeticType T, size_t S, typename D, typename E>
class VectorNegateExpression
    : public UnaryVectorExpression<T, S, D, E,
                                   VectorNegateExpression<T, S, D, E>> {
 public:
  using UnaryVectorExpression<
      T, S, D, E, VectorNegateExpression<T, S, D, E>>::UnaryVectorExpression;

  decltype(auto) get(size_t i) const { return -this->e[i]; }
};

/**
 * @brief inverse the vector
 *
 * @tparam T type
 * @tparam S size
 * @tparam D derived type
 * @tparam E expr
 * @param v vector
 * @return the inversion
 */
template <ArithmeticType T, size_t S, typename D, typename E>
inline VectorNegateExpression<T, S, D, E> negate(
    const VectorExpression<T, S, D, E> &v) {
  return VectorNegateExpression<T, S, D, E>{v};
}

/**
 * @brief vector base class
 *
 * @tparam T type
 * @tparam size dimension of the vector
 * @tparam Derived derived class type
 */
template <ArithmeticType T, size_t S, typename Derived>
class VectorBase : public VectorExpression<T, S, Derived, Derived> {
 public:
  /**
   * @brief Construct a new Vector Base object with
   * all components initialized to zero
   *
   */
  explicit VectorBase() : _data() {}

  /**
   * @brief Construct a new Vector Base object with
   * all components initialized to `value`
   *
   * @tparam Arg value type
   * @param value init value
   */
  template <ArithmeticType Arg>
  explicit VectorBase(Arg value) {
    set(value);
    DAKKU_CHECK(!hasNaNs(), "has nan");
  }

  /**
   * @brief Construct a new Vector Base object with a vector expression
   *
   * @tparam Expr expression type
   * @param expr the given expression
   */
  template <typename Expr>
  VectorBase(const VectorExpression<T, S, Derived, Expr> &expr) {
    set(expr);
    DAKKU_CHECK(!hasNaNs(), "has nan");
  }

  /**
   * @brief Construct a new Vector Base object with a vector expression
   *
   * @tparam Other
   * @tparam OtherDerived
   * @tparam Expr
   * @param expr the given expression
   */
  template <typename Other, typename OtherDerived, typename Expr>
  explicit VectorBase(
      const VectorExpression<Other, S, OtherDerived, Expr> &expr) {
    set(expr);
    DAKKU_CHECK(!hasNaNs(), "has nan");
  }

  VectorBase(const VectorBase &other) : _data(other._data) {
    DAKKU_CHECK(!hasNaNs(), "has nan");
  }
  VectorBase(VectorBase &&other) noexcept : _data(std::move(other._data)) {
    DAKKU_CHECK(!hasNaNs(), "has nan");
  }
  VectorBase &operator=(const VectorBase &other) {
    if (this == &other) return *this;
    _data = other._data;
    DAKKU_CHECK(!hasNaNs(), "has nan");
    return *this;
  }
  VectorBase &operator=(VectorBase &&other) noexcept {
    if (this == &other) return *this;
    _data = std::move(other._data);
    DAKKU_CHECK(!hasNaNs(), "has nan");
    return *this;
  }

  /**
   * @brief Construct a new Vector Base object with given values
   *
   * @tparam Args value types
   * @param args init values
   */
  template <ArithmeticType... Args>
  requires(sizeof...(Args) == S) explicit VectorBase(Args &&...args) {
    set(std::forward<Args>(args)...);
  }

  /**
   * @brief set all components to `value`
   *
   * @tparam Arg value type
   * @param value the given value
   */
  template <ArithmeticType Arg>
  void set(Arg value) {
    _data.fill(static_cast<T>(value));
  }

  /**
   * @brief set data[index] to value
   *
   * @tparam Arg value type
   * @param index the index of the component
   * @param value the given value
   */
  template <ArithmeticType Arg>
  void setByIndex(size_t index, Arg value) {
    DAKKU_CHECK(0 <= index && index < S, "index out of range: {} >= {}", index,
                S);
    _data[index] = static_cast<T>(value);
  }

  /**
   * @brief set data to args
   *
   * @tparam Args value types
   * @tparam Is indices (generated by std::index_sequence_for)
   * @param args values
   */
  template <ArithmeticType... Args, size_t... Is>
  requires(sizeof...(Args) == S) void set(std::index_sequence<Is...>,
                                          Args &&...args) {
    (setByIndex(Is, std::forward<Args>(args)), ...);
  }

  /**
   * @brief set data to args
   *
   * @tparam Args value types
   * @param args values
   */
  template <ArithmeticType... Args>
  requires(sizeof...(Args) == S) void set(Args &&...args) {
    set(std::index_sequence_for<Args...>{}, std::forward<Args>(args)...);
  }

  /**
   * @brief set data by `rhs`'s data
   *
   * @tparam Other `rhs` type
   * @tparam OtherDerived `rhs` derived type
   * @param rhs the given vector
   */
  template <ArithmeticType Other, typename OtherDerived, typename Expr>
  void set(const VectorExpression<Other, S, OtherDerived, Expr> &rhs) {
    for (size_t i = 0; i < S; ++i) _data[i] = rhs[i];
  }

  /**
   * @brief check whether there are nan components
   *
   * @return true vector contains nan
   * @return false vector does not contain nans
   */
  [[nodiscard]] bool hasNaNs() const {
    return std::any_of(std::begin(_data), std::end(_data),
                       [](T x) { return isNaN(x); });
  }

  /**
   * @brief get i'th component
   *
   * @param i the index
   * @return the value
   */
  const T &get(size_t i) const { return _data[i]; }

  /**
   * @brief convert vector to string
   *
   * @return the converted string
   */
  [[nodiscard]] std::string toString() const {
    std::string ret{"["};
    for (size_t i = 0; i < _data.size(); ++i) {
      ret += std::to_string(_data[i]);
      if (i + 1 != _data.size()) ret += ", ";
    }
    return ret + "]";
  }

  /**
   * @brief output the vector
   *
   * @param os ostream
   * @param vec the vector to output
   * @return ostream
   */
  friend std::ostream &operator<<(std::ostream &os, const VectorBase &vec) {
    return os << vec.toString();
  }

  /**
   * @brief size of the vector
   *
   * @return size
   */
  [[nodiscard]] size_t size() const { return S; }

  decltype(auto) operator-() const { return negate(*this); }

 protected:
  /// vector base data
  std::array<T, S> _data;
};

/*! @page vector_base Vector Base

dakku currently uses [expression
template](https://en.wikipedia.org/wiki/Expression_templates) to implement
vectors.
*/
}  // namespace dakku
#endif