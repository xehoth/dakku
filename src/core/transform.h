#ifndef DAKKU_CORE_TRANSFORM_H_
#define DAKKU_CORE_TRANSFORM_H_
#include <core/vector.h>
#include <core/ray.h>

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

  /**
   * @brief Construct a new Matrix 4 x 4 object with row major float vector
   *
   */
  Matrix4x4(const std::vector<float> &v) {
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j) m[i][j] = v[j * 4 + i];
  }

  decltype(auto) operator[](size_t i) const { return m[i]; }
  decltype(auto) operator[](size_t i) { return m[i]; }
  decltype(auto) operator()(size_t i, size_t j) { return m[i][j]; }
  decltype(auto) operator()(size_t i, size_t j) const { return m[i][j]; }

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

  /**
   * @brief check whether this is an identity matrix
   *
   */
  [[nodiscard]] bool isIdentity() const { return *this == Matrix4x4{}; }

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

  /**
   * @brief row major float vector
   *
   */
  [[nodiscard]] std::vector<float> toFloatVector() const {
    std::vector<float> ret(16);
    for (int i = 0; i < 4; ++i)
      for (int j = 0; j < 4; ++j) ret[j * 4 + i] = m[i][j];
    return ret;
  }

  /// matrix data
  std::array<std::array<float, 4>, 4> m;
};

class Transform {
 public:
  Transform() = default;
  Transform(const Matrix4x4 &m) : m(m), mInv(inverse(m)) {}
  Transform(const Matrix4x4 &m, const Matrix4x4 &mInv) : m(m), mInv(mInv) {}

  /**
   * @brief transform inverse
   *
   */
  friend Transform inverse(Transform &t) { return {t.mInv, t.m}; }

  friend Transform transpose(Transform &t) {
    return {transpose(t.m), transpose(t.mInv)};
  }

  bool operator==(const Transform &rhs) const {
    return m == rhs.m && mInv == rhs.mInv;
  }

  bool operator!=(const Transform &rhs) const {
    return m != rhs.m || mInv != rhs.mInv;
  }

  bool operator<(const Transform &rhs) const {
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        if (m(i, j) < rhs.m(i, j)) return true;
        if (m(i, j) > rhs.m(i, j)) return false;
      }
    }
    return false;
  }

  /**
   * @brief whether this is an identity transform
   *
   */
  [[nodiscard]] bool isIdentity() const { return m.isIdentity(); }

  /**
   * @brief Get the Matrix object
   *
   */
  [[nodiscard]] const Matrix4x4 &getMatrix() const { return m; }

  /**
   * @brief Get the Inverse Matrix object
   *
   */
  [[nodiscard]] const Matrix4x4 &getInverseMatrix() const { return mInv; }

  /**
   * @brief transform a point
   *
   */
  template <ArithmeticType T>
  inline Point3<T> operator()(const Point3<T> &p) const;

  /**
   * @brief transform a vector
   *
   */
  template <ArithmeticType T>
  inline Vector3<T> operator()(const Vector3<T> &v) const;

  /**
   * @brief transform a normal
   *
   */
  template <ArithmeticType T>
  inline Normal3<T> operator()(const Normal3<T> &n) const;

  /**
   * @brief transform a ray
   *
   */
  inline Ray operator()(const Ray &r) const;

  /**
   * @brief transform a ray differential
   *
   */
  inline RayDifferential operator()(const RayDifferential &r) const;

  Transform operator*(const Transform &rhs) const {
    return {m * rhs.m, rhs.mInv * mInv};
  }

  /**
   * @brief whether the transform swaps handedness
   *
   */
  [[nodiscard]] bool swapsHandedNess() const {
    float det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
                m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
                m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    return det < 0;
  }

 private:
  /// transform data
  Matrix4x4 m, mInv;
};

template <ArithmeticType T>
inline Point3<T> Transform::operator()(const Point3<T> &p) const {
  T x = p.x(), y = p.y(), z = p.z();
  T xp = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3];
  T yp = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3];
  T zp = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3];
  T wp = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3];
  DAKKU_CHECK(wp != 0, "divide by zero");
  if (wp == 1)
    return Point3<T>(xp, yp, zp);
  else
    return Point3<T>(xp, yp, zp) / wp;
}

template <ArithmeticType T>
inline Vector3<T> Transform::operator()(const Vector3<T> &v) const {
  T x = v.x(), y = v.y(), z = v.z();
  return Vector3<T>(m[0][0] * x + m[0][1] * y + m[0][2] * z,
                    m[1][0] * x + m[1][1] * y + m[1][2] * z,
                    m[2][0] * x + m[2][1] * y + m[2][2] * z);
}

template <ArithmeticType T>
inline Normal3<T> Transform::operator()(const Normal3<T> &n) const {
  T x = n.x(), y = n.y(), z = n.z();
  return Normal3<T>(mInv[0][0] * x + mInv[1][0] * y + mInv[2][0] * z,
                    mInv[0][1] * x + mInv[1][1] * y + mInv[2][1] * z,
                    mInv[0][2] * x + mInv[1][2] * y + mInv[2][2] * z);
}

inline Ray Transform::operator()(const Ray &r) const {
  Point3f o = (*this)(r.o);
  Vector3f d = (*this)(r.d);
  return Ray(o, d, r.tMax);
}

inline RayDifferential Transform::operator()(const RayDifferential &r) const {
  Ray tr = (*this)(static_cast<const Ray &>(r));
  RayDifferential ret(tr.o, tr.d, tr.tMax);
  ret.hasDifferentials = r.hasDifferentials;
  ret.rxOrigin = (*this)(r.rxOrigin);
  ret.ryOrigin = (*this)(r.ryOrigin);
  ret.rxDirection = (*this)(r.rxDirection);
  ret.ryDirection = (*this)(r.ryDirection);
  return ret;
}

/**
 * @brief translate
 *
 */
DAKKU_EXPORT_CORE Transform translate(const Vector3f &delta);

/**
 * @brief scale
 *
 */
DAKKU_EXPORT_CORE Transform scale(float x, float y, float z);

/**
 * @brief rotate through x axis
 *
 */
DAKKU_EXPORT_CORE Transform rotateX(float theta);

/**
 * @brief rotate through y axis
 *
 * @param theta
 * @return DAKKU_EXPORT_CORE
 */
DAKKU_EXPORT_CORE Transform rotateY(float theta);

/**
 * @brief rotate through z axis
 *
 * @param theta
 * @return DAKKU_EXPORT_CORE
 */
DAKKU_EXPORT_CORE Transform rotateZ(float theta);

/**
 * @brief rotate through `axis`
 *
 * @param theta angle
 * @param axis given axis
 */
DAKKU_EXPORT_CORE Transform rotate(float theta, const Vector3f &axis);

/**
 * @brief look at
 *
 * @param pos camera position
 * @param look camera look direction
 * @param refUp reference up vector
 */
DAKKU_EXPORT_CORE Transform lookAt(const Point3f &pos, const Point3f &look,
                                   const Vector3f &refUp);

/**
 * @brief perspective transform
 *
 * @param fov field of view
 * @param zNear far plane z
 * @param zFar near plane z
 *
 * the shorter edge mapped to [-1, 1], the longer [-ratio, ratio]
 * left handed
 */
DAKKU_EXPORT_CORE Transform perspective(float fov, float zNear, float zFar);
}  // namespace dakku
#endif