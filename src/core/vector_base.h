#ifndef DAKKU_CORE_VECTOR_BASE_H_
#define DAKKU_CORE_VECTOR_BASE_H_
#include <core/logger.h>
#include <core/lua.h>

#include <array>

namespace dakku {

/**
 * @brief vector base
 * 
 * @tparam T type
 * @tparam S size
 * @tparam D derived
 */
template <ArithmeticType T, size_t S, typename D>
class DAKKU_EXPORT_CORE VectorBase {
 public:
  /**
   * @brief Construct a new Vector Base object
   * all components initialized to zero
   *
   */
  VectorBase() : _data() {}

  /**
   * @brief Construct a new Vector Base object
   * all components initalized to `value`
   *
   * @tparam Arg value type
   * @param value init value
   */
  template <ArithmeticType Arg>
  VectorBase(Arg value) {
    set(value);
    DAKKU_CHECK(!has_nans(), "has nan");
  }

  /**
   * @brief Construct a new Vector Base object
   * all components initalized to table
   *
   */
  VectorBase(const sol::table &table) {
    for (size_t i = 0; i < S; ++i) _data[i] = table.get_or(i + 1, T{0});
    DAKKU_CHECK(!has_nans(), "has nan");
  }

  /**
   * @brief Construct a new Vector Base object with given values
   *
   * @tparam Args value types
   * @param args init values
   */
  template <ArithmeticType... Args>
  requires(sizeof...(Args) == S) VectorBase(Args &&...args) {
    set(std::forward<Args>(args)...);
  }

  /**
   * @brief Construct a new Vector Base object with a vector expression
   *
   * @tparam Other
   * @tparam OtherDerived
   * @tparam Expr
   * @param expr the given expression
   */
  template <ArithmeticType Other, typename OtherDerived>
  explicit VectorBase(const VectorBase<Other, S, OtherDerived> &other) {
    set(other);
    DAKKU_CHECK(!has_nans(), "has nan");
  }

  VectorBase(const VectorBase &other) : _data(other._data) {
    DAKKU_CHECK(!has_nans(), "has nan");
  }
  VectorBase(VectorBase &&other) noexcept : _data(std::move(other._data)) {
    DAKKU_CHECK(!has_nans(), "has nan");
  }
  VectorBase &operator=(const VectorBase &other) {
    if (this == &other) return *this;
    _data = other._data;
    DAKKU_CHECK(!has_nans(), "has nan");
    return *this;
  }
  VectorBase &operator=(VectorBase &&other) noexcept {
    if (this == &other) return *this;
    _data = std::move(other._data);
    DAKKU_CHECK(!has_nans(), "has nan");
    return *this;
  }

  /**
   * @brief convert to derived type
   *
   */
  const D &derived() const { return static_cast<const D &>(*this); }

  /**
   * @brief convert to derived type
   *
   */
  D &derived() {
    return const_cast<D &>(static_cast<const VectorBase &>(*this).derived());
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
  void set_by_index(size_t index, Arg value) {
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
    (set_by_index(Is, std::forward<Args>(args)), ...);
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
  template <ArithmeticType Other, typename OtherDerived>
  void set(const VectorBase<Other, S, OtherDerived> &rhs) {
    for (size_t i = 0; i < S; ++i) _data[i] = static_cast<T>(rhs[i]);
  }

  /**
   * @brief get i'th component
   *
   * @param i the index
   * @return the value
   */
  const T &get(size_t i) const {
    DAKKU_CHECK(0 <= i && i < S, "index out of range {} >= {}", i, S);
    return _data[i];
  }

  /**
   * @brief size of the vector
   *
   * @return size
   */
  [[nodiscard]] size_t size() const { return S; }

  /**
   * @brief convert vector to string
   *
   * @return the converted string
   */
  [[nodiscard]] std::string to_string() const {
    std::string ret{"["};
    for (size_t i = 0; i < _data.size(); ++i) {
      ret += std::to_string(_data[i]);
      if (i + 1 != _data.size()) ret += ", ";
    }
    return ret + "]";
  }

  /**
   * @brief check whether there are nan components
   *
   * @return true vector contains nan
   * @return false vector does not contain nans
   */
  [[nodiscard]] bool has_nans() const {
    return std::any_of(std::begin(_data), std::end(_data),
                       [](T x) { return isnan(x); });
  }

  /**
   * @brief get a copy
   *
   */
  D clone() const { return D{derived()}; }

  /**
   * @brief get i'th value
   *
   * @param i index
   * @return value
   */
  const T &operator[](size_t i) const { return _data[i]; }

  /**
   * @brief get i'th value
   *
   * @param i index
   * @return value
   */
  T &operator[](size_t i) {
    return const_cast<T &>(static_cast<const VectorBase &>(*this)[i]);
  }

  /**
   * @brief addition
   *
   * @param rhs another vector
   * @return this
   */
  D &operator+=(const D &rhs) {
    for (size_t i = 0; i < S; ++i) _data[i] += rhs[i];
    return derived();
  }

  /**
   * @brief addition
   *
   * @param rhs scalar
   * @return this
   */
  template <ArithmeticType V>
  D &operator+=(V rhs) {
    for (size_t i = 0; i < S; ++i) _data[i] += rhs;
    return derived();
  }

  /**
   * @brief addition
   *
   * @param rhs another vector
   * @return result
   */
  D operator+(const D &rhs) const {
    D ret = derived();
    ret += rhs;
    return ret;
  }

  /**
   * @brief addition
   *
   * @param rhs another vector
   * @return result
   */
  template <ArithmeticType V>
  D operator+(V rhs) const {
    D ret = derived();
    ret += rhs;
    return ret;
  }

  /**
   * @brief addition
   *
   * @param a $a$
   * @param b $\vec b$
   * @return elementwise add a
   */
  template <ArithmeticType U>
  friend D operator+(U a, const D &b) {
    return b + a;
  }

  /**
   * @brief subtraction
   *
   * @param rhs another vector
   * @return this
   */
  D &operator-=(const D &rhs) {
    for (size_t i = 0; i < S; ++i) _data[i] -= rhs[i];
    return derived();
  }

  /**
   * @brief subtraction
   *
   * @param rhs scalar
   * @return this
   */
  template <ArithmeticType V>
  D &operator-=(V rhs) {
    for (size_t i = 0; i < S; ++i) _data[i] -= rhs;
    return derived();
  }

  /**
   * @brief subtraction
   *
   * @param a $a$
   * @param b $\vec b$
   * @return elementwise sub
   */
  template <ArithmeticType U>
  friend D operator-(U a, const D &b) {
    D ret = -b;
    ret += a;
    return ret;
  }

  /**
   * @brief multiplication
   *
   * @param rhs another vector
   * @return this
   */
  D &operator*=(const D &rhs) {
    for (size_t i = 0; i < S; ++i) _data[i] *= rhs[i];
    return derived();
  }

  /**
   * @brief multiplication
   *
   * @param rhs scalar
   * @return this
   */
  template <ArithmeticType V>
  D &operator*=(V rhs) {
    for (size_t i = 0; i < S; ++i) _data[i] *= rhs;
    return derived();
  }

  /**
   * @brief multiplication
   *
   * @param rhs another vector
   * @return result
   */
  D operator*(const D &rhs) const {
    D ret = derived();
    ret *= rhs;
    return ret;
  }

  /**
   * @brief multiplication
   *
   * @param rhs scalar
   * @return result
   */
  template <ArithmeticType V>
  D operator*(V rhs) const {
    D ret = derived();
    ret *= rhs;
    return ret;
  }

  /**
   * @brief multiplication
   *
   * @param a $a$
   * @param b $\vec b$
   * @return elementwise mul
   */
  template <ArithmeticType U>
  friend D operator*(U a, const D &b) {
    return b * a;
  }

  /**
   * @brief division
   *
   * @param rhs another vector
   * @return this
   */
  D &operator/=(const D &rhs) {
    for (size_t i = 0; i < S; ++i) _data[i] /= rhs[i];
    return derived();
  }

  /**
   * @brief division
   *
   * @param rhs scalar
   * @return this
   */
  template <ArithmeticType V>
  D &operator/=(V rhs) {
    return derived() *= static_cast<T>(T{1} / rhs);
  }

  /**
   * @brief division
   *
   * @param rhs another vector
   * @return result
   */
  D operator/(const D &rhs) const {
    D ret = derived();
    ret /= rhs;
    return ret;
  }

  /**
   * @brief division
   *
   * @param rhs scalar
   * @return result
   */
  template <ArithmeticType U>
  D operator/(U rhs) const {
    D ret = derived();
    ret /= rhs;
    return ret;
  }

  /**
   * @brief division
   *
   * @param a $a$
   * @param b $\vec b$
   * @return elementwise div (broad cast $a$)
   */
  template <ArithmeticType U>
  friend D operator/(U a, const D &b) {
    return D(a) / b;
  }

  /**
   * @brief output the vector
   *
   * @param os ostream
   * @param vec the vector to output
   * @return ostream
   */
  friend std::ostream &operator<<(std::ostream &os, const VectorBase &vec) {
    return os << vec.to_string();
  }

 private:
  /// vector base data
  std::array<T, S> _data;
};
}  // namespace dakku

#endif