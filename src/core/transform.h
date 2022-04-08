#ifndef DAKKU_CORE_TRANSFORM_H_
#define DAKKU_CORE_TRANSFORM_H_
#include <core/vector.h>

namespace dakku {

/**
 * @brief 4x4 matrix
 *
 */
struct DAKKU_EXPORT_CORE Matrix4x4 {
  /**
   * @brief Construct a new Matrix 4 x 4 object (default identity matrix)
   *
   */
  Matrix4x4() { setIdentity(); }

  /**
   * @brief Construct a new Matrix 4 x 4 object
   *
   */
  Matrix4x4(float t00, float t01, float t02, float t03, float t10, float t11,
            float t12, float t13, float t20, float t21, float t22, float t23,
            float t30, float t31, float t32, float t33) {
    m[0][0] = t00;
    m[0][1] = t01;
    m[0][2] = t02;
    m[0][3] = t03;
    m[1][0] = t10;
    m[1][1] = t11;
    m[1][2] = t12;
    m[1][3] = t13;
    m[2][0] = t20;
    m[2][1] = t21;
    m[2][2] = t22;
    m[2][3] = t23;
    m[3][0] = t30;
    m[3][1] = t31;
    m[3][2] = t32;
    m[3][3] = t33;
  }

  decltype(auto) operator[](size_t i) const { return m[i]; }
  decltype(auto) operator[](size_t i) { return m[i]; }

  /**
   * @brief set identity
   *
   */
  void setIdentity() {
    m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] =
        m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0;
    m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
  }

  bool operator==(const Matrix4x4 &rhs) const { return m == rhs.m; }

  bool operator!=(const Matrix4x4 &rhs) const { return m != rhs.m; }

  static Matrix4x4 mul(const Matrix4x4 &a, const Matrix4x4 &b);

  Matrix4x4 operator*(const Matrix4x4 &rhs) const { return mul(*this, rhs); }

  Matrix4x4 operator*=(const Matrix4x4 &rhs) {
    *this = mul(*this, rhs);
    return *this;
  }

  /**
   * @brief transpose of the matrix
   *
   */
  friend Matrix4x4 DAKKU_EXPORT_CORE transpose(const Matrix4x4 &m);

  /**
   * @brief get the inverse matrix
   * 
   */
  friend Matrix4x4 DAKKU_EXPORT_CORE inverse(const Matrix4x4 &m);

  [[nodiscard]] std::string toString() const {
    std::string ret = "[";
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        ret += std::to_string(m[i][j]);
        if (!(i == 3 && j == 3)) ret += ", ";
      }
    }
    ret += "]";
    return ret;
  }

  friend std::ostream &operator<<(std::ostream &os, const Matrix4x4 &m) {
    return os << m.toString();
  }

  /// matrix data
  std::array<std::array<float, 4>, 4> m;
};
}  // namespace dakku
#endif