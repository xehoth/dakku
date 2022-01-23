#pragma once

#include <dakku.h>
#include <logger/logger.h>
#include <spdlog/fmt/ostr.h>
#include <enoki/array.h>
#include <type_traits>
#include <cmath>

namespace dakku {

template <ArithmeticType T, size_t size>
class Vector;
template <ArithmeticType T, size_t size>
requires(2 <= size && size <= 3) class Point;
template <ArithmeticType T, size_t size>
requires(size == 3) class Normal;

template <ArithmeticType T, size_t size, typename Derived>
class Array {
 public:
  // default: zero
  Array() { set_zero(); }

  // construct by values
  template <ArithmeticType... Args>
  requires(sizeof...(Args) == size) explicit Array(Args &&...args)
      : _data(std::forward<Args>(args)...) {
    DAKKU_CHECK(!has_nans(), "has nan");
  }

  template <ArithmeticType U, typename D>
  explicit Array(const Array<U, size, D> &array) : _data(array.data()) {}

  [[nodiscard]] bool has_nans() const {
    return enoki::any(enoki::isnan(_data));
  }

  const T &x() const {
    static_assert(size >= 1, "Array::x() requires size >= 1");
    return _data.x();
  }
  T &x() { return const_cast<T &>(static_cast<const Array &>(*this).x()); }

  const T &y() const {
    static_assert(size >= 2, "Array::y() requires size >= 2");
    return _data.y();
  }
  T &y() { return const_cast<T &>(static_cast<const Array &>(*this).y()); }

  const T &z() const {
    static_assert(size >= 3, "Array::z() requires size >= 3");
    return _data.z();
  }
  T &z() { return const_cast<T &>(static_cast<const Array &>(*this).z()); }

  const T &w() const {
    static_assert(size >= 4, "Array::z() requires size >= 4");
    return _data.w();
  }
  T &w() { return const_cast<T &>(static_cast<const Array &>(*this).w()); }

  const T &operator[](size_t i) const {
    DAKKU_CHECK(i < size, "index out of range");
    return _data[i];
  }
  T &operator[](size_t i) {
    return const_cast<T &>(static_cast<const Array &>(*this)[i]);
  }

  Derived operator+(const Derived &rhs) const {
    DAKKU_CHECK(!rhs.has_nans(), "has nan in rhs");
    return Derived(_data + rhs._data);
  }
  Derived &operator+=(const Derived &rhs) {
    DAKKU_CHECK(!rhs.has_nans(), "has nan in rhs");
    _data += rhs._data;
    return static_cast<Derived &>(*this);
  }
  Derived operator-(const Derived &rhs) const {
    DAKKU_CHECK(!rhs.has_nans(), "has nan in rhs");
    return Derived(_data - rhs._data);
  }
  Derived &operator-=(const Derived &rhs) {
    DAKKU_CHECK(!rhs.has_nans(), "has nan in rhs");
    _data -= rhs._data;
    return static_cast<Derived &>(*this);
  }
  template <ArithmeticType U>
  requires std::is_same_v<T, std::common_type_t<T, U>> Derived
  operator*(U v) const {
    DAKKU_CHECK(!isnan(v), "v is nan");
    return Derived(_data * v);
  }
  template <ArithmeticType U>
  requires std::is_same_v<T, std::common_type_t<T, U>> Derived &operator*=(
      U v) {
    DAKKU_CHECK(!isnan(v), "v is nan");
    _data *= v;
    return static_cast<Derived &>(*this);
  }
  template <ArithmeticType U>
  requires std::is_same_v<T, std::common_type_t<T, U>> Derived
  operator/(U v) const {
    DAKKU_CHECK(v != U(0), "v is zero (dividing by zero)");
    return Derived(_data / v);
  }
  template <ArithmeticType U>
  requires std::is_same_v<T, std::common_type_t<T, U>> Derived &operator/=(
      U v) {
    DAKKU_CHECK(v != U(0), "v is zero (dividing by zero)");
    _data /= v;
    return static_cast<Derived &>(*this);
  }
  Derived operator-() const { return Derived(-_data); }

  void set_zero() { _data = enoki::zero<internal_data_type>(); }

  decltype(auto) data() { return _data; }
  decltype(auto) data() const { return _data; }

  auto operator<=>(const Array &) const = default;

  template <typename OStream>
  friend OStream &operator<<(OStream &os,
                             const Array<T, size, Derived> &array) {
    return os << array._data;
  }

  Derived abs() const { return Derived(enoki::abs(_data)); }
  template <typename D>
  decltype(auto) dot(const Array<T, size, D> &rhs) const {
    DAKKU_CHECK(!has_nans() && !rhs.has_nans(), "Has nan");
    return enoki::dot(_data, rhs._data);
  }
  template <typename D>
  decltype(auto) abs_dot(const Array<T, size, D> &rhs) const {
    DAKKU_CHECK(!has_nans() && !rhs.has_nans(), "Has nan");
    return enoki::abs_dot(_data, rhs._data);
  }
  Derived cross(const Array &rhs) const {
    static_assert(size == 3, "Cross products need size = 3");
    DAKKU_CHECK(!has_nans() && !rhs.has_nans(), "Has nan");
    return Derived(enoki::cross(_data, rhs._data));
  }

  Derived normalize() const { return Derived(enoki::normalize(_data)); }
  decltype(auto) norm() const { return enoki::norm(_data); }
  decltype(auto) squared_norm() const { return enoki::squared_norm(_data); }

  T min_component() const { return enoki::hmin(_data); }
  T max_component() const { return enoki::hmax(_data); }

  Derived min(const Array &rhs) const {
    return Derived(enoki::min(_data, rhs._data));
  }
  Derived max(const Array &rhs) const {
    return Derived(enoki::max(_data, rhs._data));
  }

  Derived lerp(const Array &rhs, Float t) const {
    return Derived(enoki::lerp(_data, rhs._data, t));
  }

  Derived floor() const { return Derived(enoki::floor(_data)); }
  Derived ceil() const { return Derived(enoki::ceil(_data)); }

 protected:
  using internal_data_type = enoki::Array<T, size>;
  internal_data_type _data;

  // helper constructors: internal used for operator
  explicit Array(const internal_data_type &data) : _data(data) {
    DAKKU_CHECK(!has_nans(), "has nan");
  }
  explicit Array(internal_data_type &&data) : _data(std::move(data)) {
    DAKKU_CHECK(!has_nans(), "has nan");
  }
};

template <ArithmeticType T, size_t size, typename Derived, ArithmeticType U>
requires std::is_same_v<T, std::common_type_t<T, U>>
inline Derived operator*(U v, const Array<T, size, Derived> &array) {
  return array.operator*(v);
}

template <ArithmeticType T, size_t size>
class Vector : public Array<T, size, Vector<T, size>> {
 public:
  using Base = Array<T, size, Vector<T, size>>;
  using Base::Base;
};

template <ArithmeticType T, size_t size>
requires(2 <= size && size <= 3) class Point
    : public Array<T, size, Point<T, size>> {
 public:
  using Base = Array<T, size, Point<T, size>>;
  using Base::Base;

  Point operator+(const Vector<T, size> &v) const {
    return Base::operator+(Point(v));
  }
  Point &operator+=(const Vector<T, size> &v) {
    return Base::operator+=(Point(v));
  }
  Point operator+(const Point &p) const { return Base::operator+(p); }
  Point &operator+=(const Point &p) { return Base::operator+=(p); }
  Vector<T, size> operator-(const Point &p) const {
    return Vector<T, size>(*this) - Vector<T, size>(p);
  }
  Point operator-(const Vector<T, size> &v) const {
    return Base::operator-(Point(v));
  }
  Point &operator-=(const Vector<T, size> &v) {
    return Base::operator-=(Point(v));
  }
  Point operator-() const { return Base::operator-(); }
};

template <ArithmeticType T, size_t size>
requires(size == 3) class Normal : public Array<T, size, Normal<T, size>> {
 public:
  using Base = Array<T, size, Normal<T, size>>;
  using Base::Base;
};

template <ArithmeticType T>
using Vector2 = Vector<T, 2>;
template <ArithmeticType T>
using Vector3 = Vector<T, 3>;
template <ArithmeticType T>
using Vector4 = Vector<T, 4>;
template <ArithmeticType T>
using Point2 = Point<T, 2>;
template <ArithmeticType T>
using Point3 = Point<T, 3>;
template <ArithmeticType T>
using Normal3 = Normal<T, 3>;

using Vector2f = Vector2<Float>;
using Vector2i = Vector2<int>;
using Vector3f = Vector3<Float>;
using Vector3i = Vector3<int>;
using Point2f = Point2<Float>;
using Point2i = Point2<int>;
using Point3f = Point3<Float>;
using Point3i = Point3<int>;

using Normal3f = Normal3<Float>;

template <ArithmeticType T, size_t size, typename Derived>
inline decltype(auto) abs(const Array<T, size, Derived> &array) {
  return array.abs();
}

template <ArithmeticType T, size_t size, typename D1, typename D2>
inline decltype(auto) dot(const Array<T, size, D1> &a,
                          const Array<T, size, D2> &b) {
  return a.dot(b);
}

template <ArithmeticType T, size_t size, typename D1, typename D2>
inline decltype(auto) abs_dot(const Array<T, size, D1> &a,
                              const Array<T, size, D2> &b) {
  return a.abs_dot(b);
}

template <ArithmeticType T, typename Derived>
inline decltype(auto) cross(const Array<T, 3, Derived> &a,
                            const Array<T, 3, Derived> &b) {
  return a.cross(b);
}

template <ArithmeticType T, size_t size, typename Derived>
inline decltype(auto) normalize(const Array<T, size, Derived> &array) {
  return array.normalize();
}

template <ArithmeticType T, size_t size, typename D>
inline decltype(auto) min_component(const Array<T, size, D> &array) {
  return array.min_component();
}

template <ArithmeticType T, size_t size, typename D>
inline decltype(auto) max_component(const Array<T, size, D> &array) {
  return array.max_component();
}

template <ArithmeticType T, size_t size, typename D>
inline decltype(auto) min(const Array<T, size, D> &a,
                          const Array<T, size, D> &b) {
  return a.min(b);
}

template <ArithmeticType T, size_t size, typename D>
inline decltype(auto) max(const Array<T, size, D> &a,
                          const Array<T, size, D> &b) {
  return a.max(b);
}

template <ArithmeticType T, size_t size>
inline decltype(auto) distance(const Point<T, size> &a,
                               const Point<T, size> &b) {
  return (a - b).norm();
}

template <ArithmeticType T, size_t size>
inline float distance_squared(const Point<T, size> &a,
                              const Point<T, size> &b) {
  return (a - b).squared_norm();
}

template <ArithmeticType T, size_t size>
inline decltype(auto) lerp(Float t, const Point<T, size> &a,
                           const Point<T, size> &b) {
  return a.lerp(b, t);
}

template <ArithmeticType T, size_t size, typename D>
inline decltype(auto) floor(const Array<T, size, D> &array) {
  return array.floor();
}

template <ArithmeticType T, size_t size, typename D>
inline decltype(auto) ceil(const Array<T, size, D> &array) {
  return array.ceil();
}

// ref: v, direction should be the same as v
template <ArithmeticType T>
inline Normal3<T> face_forward(const Normal3<T> &n, const Vector3<T> &v) {
  return dot(n, v) < 0 ? -n : n;
}

template <ArithmeticType T>
inline Normal3<T> face_forward(const Normal3<T> &n, const Normal3<T> &v) {
  return dot(n, v) < 0 ? -n : n;
}

template <ArithmeticType T>
inline Normal3<T> face_forward(const Vector3<T> &n, const Vector3<T> &v) {
  return dot(n, v) < 0 ? -n : n;
}

template <ArithmeticType T>
inline Normal3<T> face_forward(const Vector3<T> &n, const Normal3<T> &v) {
  return dot(n, v) < 0 ? -n : n;
}

template <ArithmeticType T>
inline void coordinate_system(const Vector3<T> &v1, Vector3<T> &v2,
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
}

template <ArithmeticType T>
inline std::pair<Vector3<T>, Vector3<T>> coordinate_system(
    const Vector3<T> &n) {
  std::pair<Vector3<T>, Vector3<T>> ret;
  coordinate_system(n, ret.first, ret.second);
  return ret;
}
}  // namespace dakku