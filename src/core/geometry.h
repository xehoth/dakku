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
 protected:
  using internal_data_type = enoki::Array<T, size>;
  internal_data_type _data;

 public:
  Array() { _data = enoki::zero<internal_data_type>(); }

  template <ArithmeticType... Args>
  requires(sizeof...(Args) == size) explicit Array(Args &&...args)
      : _data(std::forward<Args>(args)...) {
    DAKKU_CHECK(!has_nans(), "has nan");
  }

  [[nodiscard]] bool has_nans() const {
    return enoki::any(enoki::isnan(_data));
  }

  explicit Array(const internal_data_type &data) : _data(data) {}
  explicit Array(internal_data_type &&data) : _data(std::move(data)) {}

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

  Derived operator+(const Derived &rhs) const {
    return Derived(_data + rhs._data);
  }
  Derived &operator+=(const Derived &rhs) {
    _data += rhs._data;
    return static_cast<Derived &>(*this);
  }
};

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
};

template <ArithmeticType T, size_t size>
requires(size == 3) class Normal : public Array<T, size, Normal<T, size>> {
 public:
  using Base = Array<T, size, Normal<T, size>>;
  using Base::Array;
};

using Vector2f = Vector<Float, 2>;
using Vector2i = Vector<int, 2>;
using Vector3f = Vector<Float, 3>;
using Vector3i = Vector<int, 3>;
using Point2f = Point<Float, 2>;
using Point2i = Point<int, 2>;
using Point3f = Point<Float, 3>;
using Point3i = Point<int, 3>;
using Normal3f = Normal<Float, 3>;

// Subtracting two points should always yield a vector
// template <typename T1, size_t S1, typename T2, size_t S2>
// auto operator-(const Point<T1, S1> &p1, const Point<T2, S2> &p2) {
//  return Vector<T1, S1>(p1) - Vector<T2, S2>(p2);
//}
//
//// Subtracting a vector from a point should always yield a point
// template <typename T1, size_t S1, typename T2, size_t S2>
// auto operator-(const Point<T1, S1> &p1, const Vector<T2, S2> &v2) {
//   return p1 - Point<T2, S2>(v2);
// }
//
//// Adding a vector to a point should always yield a point
// template <typename T1, size_t S1, typename T2, size_t S2>
// auto operator+(const Point<T1, S1> &p1, const Vector<T2, S2> &v2) {
//   return p1 + Point<T2, S2>(v2);
// }

// class Vector2 : public Eigen::Matrix<T, 2, 1> {
//  public:
//   using super = Eigen::Matrix<T, 2, 1>;
//   Vector2() { this->setZero(); }
//   template <ArithmeticType S1, ArithmeticType S2>
//   explicit Vector2(S1 x, S2 y) : super(x, y) {
//     DAKKU_CHECK(!has_nans(), "Has nan in vector");
//   }
//   [[nodiscard]] bool has_nans() const {
//     return isnan(this->x()) || isnan(this->y());
//   }
//   template <typename U>
//   requires std::is_convertible_v<U, T>
//   explicit Vector2(const Point2<U> &p) : Vector2(p.x(), p.y()) {}
//   template <typename U>
//   requires std::is_convertible_v<U, T>
//   explicit Vector2(const Point3<U> &p) : Vector2(p.x(), p.y()) {}
//   template <typename EigenExpr>
//   explicit Vector2(const EigenExpr &v) : super(v) {}
//
//   Vector2 operator+(const Vector2 &v) const {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     return Vector2(static_cast<const super &>(*this) +
//                    static_cast<const super &>(v));
//   }
//
//   Vector2 &operator+=(const Vector2 &v) {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     static_cast<super &>(*this) += static_cast<const super &>(v);
//     return *this;
//   }
//
//   Vector2 operator-(const Vector2 &v) const {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     return Vector2(static_cast<const super &>(*this) -
//                    static_cast<const super &>(v));
//   }
//
//   Vector2 &operator-=(const Vector2 &v) {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     static_cast<super &>(*this) -= static_cast<const super &>(v);
//     return *this;
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Vector2
//   operator*(U f) const {
//     return Vector2(static_cast<const super &>(*this) * static_cast<T>(f));
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Vector2 &operator*=(
//       U f) {
//     static_cast<super &>(*this) *= f;
//     return *this;
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Vector2
//   operator/(U f) const {
//     return Vector2(static_cast<const super &>(*this) / static_cast<T>(f));
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Vector2 &operator/=(
//       U f) {
//     static_cast<super &>(*this) /= f;
//     return *this;
//   }
//
//   Vector2 operator-() const {
//     return Vector2(-static_cast<const super &>(*this));
//   }
//
//   template <typename OStream>
//   friend OStream &operator<<(OStream &os, const Vector2<T> &v) {
//     return os << "[" << v.x() << ", " << v.y() << "]";
//   }
// };
//
// template <ArithmeticType T>
// class Vector3 : public Eigen::Matrix<T, 3, 1> {
//  public:
//   using super = Eigen::Matrix<T, 3, 1>;
//   Vector3() { this->setZero(); }
//   template <ArithmeticType S1, ArithmeticType S2, ArithmeticType S3>
//   explicit Vector3(S1 x, S2 y, S3 z) : super(x, y, z) {
//     DAKKU_CHECK(!has_nans(), "Has nan in vector");
//   }
//   [[nodiscard]] bool has_nans() const {
//     return isnan(this->x()) || isnan(this->y()) || isnan(this->z());
//   }
//   explicit Vector3(const Point3<T> &p) : Vector3(p.x(), p.y(), p.z()) {}
//   explicit Vector3(const Normal3<T> &n) : Vector3(n.x(), n.y(), n.z()) {}
//   template <typename EigenExpr>
//   explicit Vector3(const EigenExpr &v) : super(v) {}
//
//   Vector3 operator+(const Vector3 &v) const {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     return Vector3(static_cast<const super &>(*this) +
//                    static_cast<const super &>(v));
//   }
//
//   Vector3 &operator+=(const Vector3 &v) {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     static_cast<super &>(*this) += v;
//     return *this;
//   }
//
//   Vector3 operator-(const Vector3 &v) const {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     return Vector3(static_cast<const super &>(*this) -
//                    static_cast<const super &>(v));
//   }
//
//   Vector3 &operator-=(const Vector3 &v) {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     static_cast<super &>(*this) -= v;
//     return *this;
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Vector3
//   operator*(U f) const {
//     return Vector3(static_cast<const super &>(*this) * static_cast<T>(f));
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Vector3 &operator*=(
//       U f) {
//     static_cast<super &>(*this) *= f;
//     return *this;
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Vector3
//   operator/(U f) const {
//     return Vector3(static_cast<const super &>(*this) / static_cast<T>(f));
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Vector3 &operator/=(
//       U f) {
//     static_cast<super &>(*this) /= f;
//     return *this;
//   }
//
//   Vector3 operator-() const {
//     return Vector3(-static_cast<const super &>(*this));
//   }
//
//   template <typename OStream>
//   friend OStream &operator<<(OStream &os, const Vector3<T> &v) {
//     return os << "[" << v.x() << ", " << v.y() << ", " << v.z() << "]";
//   }
// };
//
// using Vector2f = Vector2<float>;
// using Vector2i = Vector2<int>;
// using Vector3f = Vector3<float>;
// using Vector3i = Vector3<int>;
//
// template <ArithmeticType T>
// class Point2 : public Eigen::Matrix<T, 2, 1> {
//  public:
//   using super = Eigen::Matrix<T, 2, 1>;
//   Point2() { this->setZero(); }
//   template <ArithmeticType S1, ArithmeticType S2>
//   explicit Point2(S1 x, S2 y) : super(x, y) {
//     DAKKU_CHECK(!has_nans(), "Has nan in point");
//   }
//   template <typename EigenExpr>
//   explicit Point2(const EigenExpr &v) : super(v) {}
//
//   [[nodiscard]] bool has_nans() const {
//     return isnan(this->x()) || isnan(this->y());
//   }
//   explicit Point2(const Point3<T> &p) : Point2(p.x(), p.y()) {
//     DAKKU_CHECK(!p.has_nans(), "Has nan in point");
//   }
//   template <typename U>
//   requires std::is_convertible_v<U, T>
//   explicit Point2(const Point2<U> &p) : Point2(p.x(), p.y()) {}
//
//   template <typename U>
//   requires std::is_convertible_v<U, T>
//   explicit Point2(const Vector2<U> &v) : Point2(v.x(), v.y()) {}
//
//   Point2 operator+(const Vector2<T> &v) const {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     return Point2(static_cast<const super &>(*this) +
//                   static_cast<const super &>(v));
//   }
//
//   Point2 &operator+=(const Vector2<T> &v) {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     static_cast<super &>(*this) += static_cast<const super &>(v);
//     return *this;
//   }
//
//   Vector2<T> operator-(const Point2 &p) const {
//     DAKKU_CHECK(!p.has_nans(), "Has nan in point");
//     return Vector2<T>(static_cast<const super &>(*this) -
//                       static_cast<const super &>(p));
//   }
//
//   Point2 operator-(const Vector2<T> &v) const {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     return Point2<T>(static_cast<const super &>(*this) -
//                      static_cast<const super &>(v));
//   }
//
//   Point2 operator-() const {
//     return Point2<T>(-static_cast<const super &>(*this));
//   }
//
//   Point2<T> &operator-=(const Vector2<T> &v) {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     static_cast<super &>(*this) -= static_cast<const super &>(v);
//     return *this;
//   }
//
//   Point2<T> operator+(const Point2<T> &p) const {
//     DAKKU_CHECK(!p.has_nans(), "Has nan in point");
//     return Point2<T>(static_cast<const super &>(*this) +
//                      static_cast<const super &>(p));
//   }
//
//   Point2<T> &operator+=(const Point2<T> &p) {
//     DAKKU_CHECK(!p.has_nans(), "Has nan in point");
//     static_cast<super &>(*this) += static_cast<const super &>(p);
//     return *this;
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Point2<T>
//   operator*(U f) const {
//     return Point2<T>(static_cast<const super &>(*this) * static_cast<T>(f));
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Point2<T>
//   &operator*=(U f) {
//     static_cast<super &>(*this) *= static_cast<T>(f);
//     return *this;
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Point2<T>
//   operator/(U f) const {
//     return Point2<T>(static_cast<const super &>(*this) / static_cast<T>(f));
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Point2<T>
//   &operator/=(U f) {
//     static_cast<super &>(*this) /= static_cast<T>(f);
//     return *this;
//   }
//
//   template <typename OStream>
//   friend OStream &operator<<(OStream &os, const Point2<T> &p) {
//     return os << "[" << p.x() << ", " << p.y() << "]";
//   }
// };
//
// template <ArithmeticType T>
// class Point3 : public Eigen::Matrix<T, 3, 1> {
//  public:
//   using super = Eigen::Matrix<T, 3, 1>;
//   Point3() { this->setZero(); }
//   template <ArithmeticType S1, ArithmeticType S2, ArithmeticType S3>
//   explicit Point3(S1 x, S2 y, S3 z) : super(x, y, z) {
//     DAKKU_CHECK(!has_nans(), "Has nan in point");
//   }
//   template <typename EigenExpr>
//   explicit Point3(const EigenExpr &v) : super(v) {}
//
//   [[nodiscard]] bool has_nans() const {
//     return isnan(this->x()) || isnan(this->y()) || isnan(this->z());
//   }
//   template <typename U>
//   requires std::is_convertible_v<U, T>
//   explicit Point3(const Point3<U> &p) : Point3(p.x(), p.y(), p.z()) {}
//
//   template <typename U>
//   requires std::is_convertible_v<U, T>
//   explicit Point3(const Vector3<U> &v) : Point3(v.x(), v.y(), v.z()) {}
//
//   Point3 operator+(const Vector3<T> &v) const {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     return Point3(static_cast<const super &>(*this) +
//                   static_cast<const super &>(v));
//   }
//
//   Point3 &operator+=(const Vector3<T> &v) {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     static_cast<super &>(*this) += static_cast<const super &>(v);
//     return *this;
//   }
//
//   Vector3<T> operator-(const Point3 &p) const {
//     DAKKU_CHECK(!p.has_nans(), "Has nan in point");
//     return Vector3<T>(static_cast<const super &>(*this) -
//                       static_cast<const super &>(p));
//   }
//
//   Point3 operator-(const Vector3<T> &v) const {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     return Point3<T>(static_cast<const super &>(*this) -
//                      static_cast<const super &>(v));
//   }
//
//   Point3 operator-() const {
//     return Point3<T>(-static_cast<const super &>(*this));
//   }
//
//   Point3<T> &operator-=(const Vector3<T> &v) {
//     DAKKU_CHECK(!v.has_nans(), "Has nan in vector");
//     static_cast<super &>(*this) -= static_cast<const super &>(v);
//     return *this;
//   }
//
//   Point3<T> operator+(const Point3<T> &p) const {
//     DAKKU_CHECK(!p.has_nans(), "Has nan in point");
//     return Point3<T>(static_cast<const super &>(*this) +
//                      static_cast<const super &>(p));
//   }
//
//   Point3<T> &operator+=(const Point3<T> &p) {
//     DAKKU_CHECK(!p.has_nans(), "Has nan in point");
//     static_cast<super &>(*this) += static_cast<const super &>(p);
//     return *this;
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Point3<T>
//   operator*(U f) const {
//     return Point3<T>(static_cast<const super &>(*this) * static_cast<T>(f));
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Point3<T>
//   &operator*=(U f) {
//     static_cast<super &>(*this) *= static_cast<T>(f);
//     return *this;
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Point3<T>
//   operator/(U f) const {
//     return Point3<T>(static_cast<const super &>(*this) / static_cast<T>(f));
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Point3<T>
//   &operator/=(U f) {
//     static_cast<super &>(*this) /= static_cast<T>(f);
//     return *this;
//   }
//
//   template <typename OStream>
//   friend OStream &operator<<(OStream &os, const Point3<T> &p) {
//     return os << "[" << p.x() << ", " << p.y() << ", " << p.z() << "]";
//   }
// };
//
// using Point2f = Point2<float>;
// using Point2i = Point2<int>;
// using Point3f = Point3<float>;
// using Point3i = Point3<int>;
//
// template <ArithmeticType T>
// class Normal3 : public Eigen::Matrix<T, 3, 1> {
//  public:
//   using super = Eigen::Matrix<T, 3, 1>;
//
//   Normal3() { this->setZero(); }
//   template <ArithmeticType S1, ArithmeticType S2, ArithmeticType S3>
//   explicit Normal3(S1 x, S2 y, S3 z) : super(x, y, z) {
//     DAKKU_CHECK(!has_nans(), "Has nan in normal");
//   }
//
//   Normal3 operator-() const {
//     return Normal3(-static_cast<const super &>(*this));
//   }
//   template <typename EigenExpr>
//   explicit Normal3(const EigenExpr &v) : super(v) {}
//
//   [[nodiscard]] bool has_nans() const {
//     return isnan(this->x()) || isnan(this->y()) || isnan(this->z());
//   }
//   template <typename U>
//   requires std::is_convertible_v<U, T>
//   explicit Normal3(const Point3<U> &p) : Normal3(p.x(), p.y(), p.z()) {}
//
//   template <typename U>
//   requires std::is_convertible_v<U, T>
//   explicit Normal3(const Vector3<U> &v) : Normal3(v.x(), v.y(), v.z()) {}
//
//   Normal3 operator+(const Normal3 &n) const {
//     DAKKU_CHECK(!n.has_nans(), "Has nan in normal");
//     return Normal3(static_cast<const super &>(*this) +
//                    static_cast<const super &>(n));
//   }
//
//   Normal3 &operator+=(const Normal3 &n) {
//     DAKKU_CHECK(!n.has_nans(), "Has nan in normal");
//     static_cast<super &>(*this) += static_cast<const super &>(n);
//     return *this;
//   }
//
//   Normal3 operator-(const Normal3 &n) const {
//     DAKKU_CHECK(!n.has_nans(), "Has nan in normal");
//     return Normal3(static_cast<const super &>(*this) -
//                    static_cast<const super &>(n));
//   }
//
//   Normal3 &operator-=(const Normal3 &n) {
//     DAKKU_CHECK(!n.has_nans(), "Has nan in normal");
//     static_cast<super &>(*this) -= static_cast<const super &>(n);
//     return *this;
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Normal3<T>
//   operator*(U f) const {
//     return Normal3<T>(static_cast<const super &>(*this) * static_cast<T>(f));
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Normal3<T>
//   &operator*=(U f) {
//     static_cast<super &>(*this) *= static_cast<T>(f);
//     return *this;
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Normal3<T>
//   operator/(U f) const {
//     return Normal3<T>(static_cast<const super &>(*this) / static_cast<T>(f));
//   }
//
//   template <ArithmeticType U>
//   requires std::is_same_v<std::common_type_t<T, U>, T> Normal3<T>
//   &operator/=(U f) {
//     static_cast<super &>(*this) /= static_cast<T>(f);
//     return *this;
//   }
//
//   template <typename OStream>
//   friend OStream &operator<<(OStream &os, const Normal3<T> &n) {
//     return os << "[" << n.x() << ", " << n.y() << ", " << n.z() << "]";
//   }
// };
//
// using Normal3f = Normal3<float>;
//
// template <ArithmeticType T, ArithmeticType U>
// inline Vector3<T> operator*(U s, const Vector3<T> &v) {
//   return v * s;
// }
//
// template <ArithmeticType T>
// inline Vector3<T> abs(const Vector3<T> &v) {
//   return Vector3<T>(v.cwiseAbs());
// }
//
// template <ArithmeticType T>
// inline T dot(const Vector3<T> &a, const Vector3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan in vector");
//   return a.dot(b);
// }
//
// template <ArithmeticType T>
// inline T abs_dot(const Vector3<T> &a, const Vector3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan in vector");
//   return std::abs(dot(a, b));
// }
//
// template <ArithmeticType T>
// inline Vector3<T> cross(const Vector3<T> &a, const Vector3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan");
//   return Vector3<T>(a.cross(b));
// }
//
// template <ArithmeticType T>
// inline Vector3<T> cross(const Vector3<T> &a, const Normal3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan");
//   return Vector3<T>(a.cross(b));
// }
//
// template <ArithmeticType T>
// inline Vector3<T> cross(const Normal3<T> &a, const Vector3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan");
//   return Vector3<T>(a.cross(b));
// }
//
// template <ArithmeticType T>
// inline Vector3<T> normalize(const Vector3<T> &v) {
//   return Vector3<T>(v.normalized());
// }
//
// template <ArithmeticType T>
// inline T min_component(const Vector3<T> &v) {
//   return v.minCoeff();
// }
//
// template <ArithmeticType T>
// inline T max_component(const Vector3<T> &v) {
//   return v.maxCoeff();
// }
//
// template <ArithmeticType T>
// inline int max_dimension(const Vector3<T> &v) {
//   return (v.x() > v.y()) ? ((v.x() > v.z()) ? 0 : 2)
//                          : ((v.y() > v.z()) ? 1 : 2);
// }
//
// template <ArithmeticType T>
// inline Vector3<T> min(const Vector3<T> &a, const Vector3<T> &b) {
//   return Vector3<T>(a.cwiseMin(b));
// }
//
// template <ArithmeticType T>
// inline Vector3<T> max(const Vector3<T> &a, const Vector3<T> &b) {
//   return Vector3<T>(a.cwiseMax(b));
// }
//
// template <ArithmeticType T>
// inline Vector3<T> permute(const Vector3<T> &v, int x, int y, int z) {
//   return Vector3<T>(v[x], v[y], v[z]);
// }
//
// template <ArithmeticType T, ArithmeticType U>
// inline Vector2<T> operator*(U f, const Vector2<T> &v) {
//   return v * f;
// }
//
// template <ArithmeticType T>
// inline T dot(const Vector2<T> &a, const Vector2<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan in vector");
//   return a.dot(b);
// }
//
// template <ArithmeticType T>
// inline T abs_dot(const Vector2<T> &a, const Vector2<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan in vector");
//   return std::abs(dot(a, b));
// }
//
// template <ArithmeticType T>
// inline Vector2<T> normalize(const Vector2<T> &v) {
//   return Vector2<T>(v.normalized());
// }
//
// template <ArithmeticType T>
// inline Vector2<T> abs(const Vector2<T> &v) {
//   return Vector2<T>(v.cwiseAbs());
// }
//
// template <ArithmeticType T>
// inline float distance(const Point3<T> &a, const Point3<T> &b) {
//   return (a - b).norm();
// }
//
// template <ArithmeticType T>
// inline float distance_squared(const Point3<T> &a, const Point3<T> &b) {
//   return (a - b).squaredNorm();
// }
//
// template <ArithmeticType T, ArithmeticType U>
// inline Point3<T> operator*(U f, const Point3<T> &p) {
//   return p * f;
// }
//
// template <ArithmeticType T>
// inline Point3<T> lerp(float t, const Point3<T> &p0, const Point3<T> &p1) {
//   return (1.0f - t) * p0 + t * p1;
// }
//
// template <ArithmeticType T>
// inline Point3<T> min(const Point3<T> &a, const Point3<T> &b) {
//   return Point3<T>(a.cwiseMin(b));
// }
//
// template <ArithmeticType T>
// inline Point3<T> max(const Point3<T> &a, const Point3<T> &b) {
//   return Point3<T>(a.cwiseMax(b));
// }
//
// template <ArithmeticType T>
// inline Point3<T> floor(const Point3<T> &p) {
//   return Point3<T>(std::floor(p.x()), std::floor(p.y()), std::floor(p.z()));
// }
//
// template <ArithmeticType T>
// inline Point3<T> ceil(const Point3<T> &p) {
//   return Point3<T>(std::ceil(p.x()), std::ceil(p.y()), std::ceil(p.z()));
// }
//
// template <ArithmeticType T>
// inline Point3<T> abs(const Point3<T> &p) {
//   return Point3<T>(p.cwiseAbs());
// }
//
// template <ArithmeticType T>
// inline float distance(const Point2<T> &p1, const Point2<T> &p2) {
//   return (p1 - p2).norm();
// }
//
// template <ArithmeticType T>
// inline float distance_squared(const Point2<T> &p1, const Point2<T> &p2) {
//   return (p1 - p2).squaredNorm();
// }
//
// template <ArithmeticType T, ArithmeticType U>
// inline Point2<T> operator*(U f, const Point2<T> &p) {
//   return p * f;
// }
//
// template <ArithmeticType T>
// inline Point2<T> floor(const Point2<T> &p) {
//   return Point2<T>(std::floor(p.x()), std::floor(p.y()));
// }
//
// template <ArithmeticType T>
// inline Point2<T> ceil(const Point2<T> &p) {
//   return Point2<T>(std::ceil(p.x()), std::ceil(p.y()));
// }
//
// template <ArithmeticType T>
// inline Point2<T> lerp(float t, const Point2<T> &v0, const Point2<T> &v1) {
//   return (1.0f - t) * v0 + t * v1;
// }
//
// template <ArithmeticType T>
// inline Point2<T> min(const Point2<T> &a, const Point2<T> &b) {
//   return Point2<T>(a.cwiseMin(b));
// }
//
// template <ArithmeticType T>
// inline Point2<T> max(const Point2<T> &a, const Point2<T> &b) {
//   return Point2<T>(a.cwiseMax(b));
// }
//
// template <ArithmeticType T>
// inline Point3<T> permute(const Point3<T> &p, int x, int y, int z) {
//   return Point3<T>(p[x], p[y], p[z]);
// }
//
// template <ArithmeticType T, ArithmeticType U>
// inline Normal3<T> operator*(U f, const Normal3<T> &n) {
//   return n * f;
// }
//
// template <ArithmeticType T>
// inline Normal3<T> normalize(const Normal3<T> &n) {
//   return Normal3<T>(n.normalized());
// }
//
// template <ArithmeticType T>
// inline T dot(const Normal3<T> &a, const Normal3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan");
//   return a.dot(b);
// }
//
// template <ArithmeticType T>
// inline T dot(const Vector3<T> &a, const Normal3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan");
//   return a.dot(b);
// }
//
// template <ArithmeticType T>
// inline T dot(const Normal3<T> &a, const Vector3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan");
//   return a.dot(b);
// }
//
// template <ArithmeticType T>
// inline T abs_dot(const Normal3<T> &a, const Normal3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan");
//   return std::abs(dot(a, b));
// }
//
// template <ArithmeticType T>
// inline T abs_dot(const Normal3<T> &a, const Vector3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan");
//   return std::abs(dot(a, b));
// }
//
// template <ArithmeticType T>
// inline T abs_dot(const Vector3<T> &a, const Normal3<T> &b) {
//   DAKKU_CHECK(!a.has_nans() && !b.has_nans(), "Has nan");
//   return std::abs(dot(a, b));
// }
//
//// ref: v, direction should be the same as v
// template <ArithmeticType T>
// inline Normal3<T> face_forward(const Normal3<T> &n, const Vector3<T> &v) {
//   return dot(n, v) < 0.0f ? -n : n;
// }
//
// template <ArithmeticType T>
// inline Normal3<T> face_forward(const Normal3<T> &n, const Normal3<T> &v) {
//   return dot(n, v) < 0.0f ? -n : n;
// }
//
// template <ArithmeticType T>
// inline Normal3<T> face_forward(const Vector3<T> &n, const Vector3<T> &v) {
//   return dot(n, v) < 0.0f ? -n : n;
// }
//
// template <ArithmeticType T>
// inline Normal3<T> face_forward(const Vector3<T> &n, const Normal3<T> &v) {
//   return dot(n, v) < 0.0f ? -n : n;
// }
//
// template <ArithmeticType T>
// inline Normal3<T> abs(const Normal3<T> &v) {
//   return Normal3<T>(v.cwiseAbs());
// }
//
// template <ArithmeticType T>
// inline void coordinate_system(const Vector3<T> &v1, Vector3<T> &v2,
//                               Vector3<T> &v3) {
//   // choose larger one to get better precision
//   if (std::abs(v1.x()) > std::abs(v1.y())) {
//     // take v2 as (-z, 0, x), (-z, 0, x) \cdot (x, y, z) = 0
//     v2 = Vector3<T>(-v1.z(), 0, v1.x()) /
//          std::sqrt(v1.x() * v1.x() + v1.z() * v1.z());
//   } else {
//     // similar
//     v2 = Vector3<T>(0, v1.z(), -v1.y()) /
//          std::sqrt(v1.y() * v1.y() + v1.z() * v1.z());
//   }
// }
}  // namespace dakku