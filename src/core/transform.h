#pragma once

#include <dakku.h>
#include <core/vector.h>
#include <core/ray.h>
#include <enoki/matrix.h>
#include <enoki/transform.h>

namespace dakku {

struct Matrix4x4 {
  Matrix4x4() : _data(enoki::identity<enoki::Matrix<Float, 4>>()) {}
  template <ArithmeticType... Args>
  requires(sizeof...(Args) == 16) explicit Matrix4x4(Args &&...args)
      : _data(std::forward<Args>(args)...) {}

  bool operator==(const Matrix4x4 &rhs) const { return _data == rhs._data; }
  bool operator!=(const Matrix4x4 &rhs) const { return _data != rhs._data; }

  Matrix4x4 operator*(const Matrix4x4 &rhs) const {
    return Matrix4x4(_data * rhs._data);
  }
  Vector4<Float> operator*(const Vector4<Float> &v) {
    return Vector4<Float>(_data * v._data);
  }

  friend Matrix4x4 transpose(const Matrix4x4 &m) {
    return Matrix4x4(enoki::transpose(m._data));
  }

  friend Matrix4x4 inverse(const Matrix4x4 &m) {
    return Matrix4x4(enoki::inverse(m._data));
  }

  template <typename OStream>
  friend OStream &operator<<(OStream &os, const Matrix4x4 &m) {
    return os << m._data;
  }

  decltype(auto) operator()(size_t r, size_t c) { return _data[r][c]; }
  decltype(auto) operator()(size_t r, size_t c) const { return _data[r][c]; }

  friend class Transform;

 protected:
  explicit Matrix4x4(const enoki::Matrix<Float, 4> &data) : _data(data) {}
  enoki::Matrix<Float, 4> _data;
};

class Transform {
 public:
  Transform() = default;
  explicit Transform(const Matrix4x4 &m) : m(m), m_inv(inverse(m)) {}
  explicit Transform(const Matrix4x4 &m, const Matrix4x4 &m_inv)
      : m(m), m_inv(m_inv) {}

  friend Transform inverse(const Transform &t) {
    return Transform(t.m_inv, t.m);
  }
  friend Transform transpose(const Transform &t) {
    return Transform(transpose(t.m), transpose(t.m_inv));
  }
  bool operator==(const Transform &rhs) const {
    return m == rhs.m && m_inv == rhs.m_inv;
  }
  bool operator!=(const Transform &rhs) const {
    return m != rhs.m || m_inv != rhs.m_inv;
  }
  bool operator<(const Transform &rhs) const {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        if (m(i, j) != rhs.m(i, j)) return m(i, j) < rhs.m(i, j);
      }
    }
    return false;
  }
  [[nodiscard]] bool is_identity() const { return m == Matrix4x4(); }
  [[nodiscard]] const Matrix4x4 &get_matrix() const { return m; }
  [[nodiscard]] const Matrix4x4 &get_inverse_matrix() const { return m_inv; }
  [[nodiscard]] bool has_scale() const {}
  template <ArithmeticType T>
  Point3<T> operator()(const Point3<T> &p) const {
    T x = p.x(), y = p.y(), z = p.z();
    T xp = m(0, 0) * x + m(0, 1) * y + m(0, 2) * z + m(0, 3);
    T yp = m(1, 0) * x + m(1, 1) * y + m(1, 2) * z + m(1, 3);
    T zp = m(2, 0) * x + m(2, 1) * y + m(2, 2) * z + m(2, 3);
    T wp = m(3, 0) * x + m(3, 1) * y + m(3, 2) * z + m(3, 3);
    DAKKU_CHECK(wp != 0, "w is zero");
    if (wp == 1)
      return Point3<T>(xp, yp, zp);
    else
      return Point3<T>(xp, yp, zp) / wp;
  }
  template <ArithmeticType T>
  Vector3<T> operator()(const Vector3<T> &v) const {
    T x = v.x(), y = v.y(), z = v.z();
    return Vector3<T>(m(0, 0) * x + m(0, 1) * y + m(0, 2) * z,
                      m(1, 0) * x + m(1, 1) * y + m(1, 2) * z,
                      m(2, 0) * x + m(2, 1) * y + m(2, 2) * z);
  }
  template <ArithmeticType T>
  Normal3<T> operator()(const Normal3<T> &n) const {
    T x = n.x(), y = n.y(), z = n.z();
    return Normal3<T>(m_inv(0, 0) * x + m_inv(1, 0) * y + m_inv(2, 0) * z,
                      m_inv(0, 1) * x + m_inv(1, 1) * y + m_inv(2, 1) * z,
                      m_inv(0, 2) * x + m_inv(1, 2) * y + m_inv(2, 2) * z);
  }
  Ray operator()(const Ray &r) const {
    Point3f o = (*this)(r.o);
    Vector3f d = (*this)(r.d);
    return Ray(o, d, r.t_max);
  }

 private:
  Matrix4x4 m, m_inv;
};

}  // namespace dakku