#ifndef DAKKU_SRC_CORE_TRANSFORM_H_
#define DAKKU_SRC_CORE_TRANSFORM_H_
#include <core/fwd.h>
#include <core/serialization.h>
#include <core/vector.h>
#include <core/ray.h>
#include <Eigen/Dense>
#include <vector>

DAKKU_BEGIN

struct Matrix4x4 final {
  explicit Matrix4x4() { _data.setIdentity(); }
  explicit Matrix4x4(const Float mat[4][4]);
  explicit Matrix4x4(Float t00, Float t01, Float t02, Float t03, Float t10,
                     Float t11, Float t12, Float t13, Float t20, Float t21,
                     Float t22, Float t23, Float t30, Float t31, Float t32,
                     Float t33);
  bool operator==(const Matrix4x4 &rhs) const { return _data == rhs._data; }
  bool operator!=(const Matrix4x4 &rhs) const { return _data != rhs._data; }
  friend Matrix4x4 transpose(const Matrix4x4 &);
  friend Matrix4x4 inverse(const Matrix4x4 &);
  Matrix4x4 operator*(const Matrix4x4 &rhs) const {
    return Matrix4x4(_data * rhs._data);
  }
  Matrix4x4 &operator*=(const Matrix4x4 &rhs) {
    _data *= rhs._data;
    return *this;
  }

  friend void to_json(Json &json, const Matrix4x4 &mat) {
    internalType out = mat._data.transpose();
    json = std::vector<Float>{out.data(), out.data() + 16};
  }

  friend void from_json(const Json &json, Matrix4x4 &mat) {
    Float tmp[16];
    json.get_to(tmp);
    mat._data = Eigen::Map<internalType>(tmp, 4, 4).transpose();
  }

  friend std::ostream &operator<<(std::ostream &os, const Matrix4x4 &mat) {
    os << "[\n";
    for (int i = 0; i < 4; ++i) {
      os << "  [";
      for (int j = 0; j < 4; ++j) {
        os << mat._data(i, j);
        if (j != 3) os << ", ";
      }
      os << "]\n";
    }
    os << "]";
    return os;
  }
  decltype(auto) operator()(int r, int c) const { return _data(r, c); }
  decltype(auto) operator()(int r, int c) { return _data(r, c); }
  [[nodiscard]] bool isIdentity() const { return _data.isIdentity(); }

  friend class Transform;

 private:
  using internalType = Eigen::Matrix<Float, 4, 4>;
  explicit Matrix4x4(const internalType &data) : _data(data) {}
  explicit Matrix4x4(internalType &&data) : _data(std::move(data)) {}
  template <template <typename> typename EigenTmp, typename OtherDerived>
  requires(
      std::is_same_v<EigenTmp<OtherDerived>, Eigen::EigenBase<OtherDerived>> ||
      std::is_same_v<EigenTmp<OtherDerived>, Eigen::DenseBase<OtherDerived>> ||
      std::is_same_v<
          EigenTmp<OtherDerived>,
          Eigen::ReturnByValue<
              OtherDerived>>) explicit Matrix4x4(const EigenTmp<OtherDerived>
                                                     &data)
      : _data(data) {}
  internalType _data;
};

class Transform final {
 public:
  explicit Transform() = default;
  explicit Transform(const Float mat[4][4]) : m(mat), mInv(inverse(m)) {}
  explicit Transform(const Matrix4x4 &m) : m(m), mInv(inverse(m)) {}
  explicit Transform(const Matrix4x4 &m, const Matrix4x4 &mInv)
      : m(m), mInv(mInv) {}

  friend Transform inverse(const Transform &t) {
    return Transform(t.mInv, t.m);
  }
  friend Transform transpose(const Transform &t) {
    return Transform(transpose(t.m), transpose(t.mInv));
  }
  bool operator==(const Transform &t) const {
    return t.m == m && t.mInv == mInv;
  }
  bool operator!=(const Transform &t) const {
    return t.m != m || t.mInv != mInv;
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
  [[nodiscard]] bool isIdentity() const { return m.isIdentity(); }
  [[nodiscard]] const Matrix4x4 &getMatrix() const { return m; }
  [[nodiscard]] const Matrix4x4 &getInverseMatrix() const { return mInv; }
  [[nodiscard]] bool hasScale() const {
    Float la2 = (*this)(Vector3f(1, 0, 0)).squaredNorm();
    Float lb2 = (*this)(Vector3f(0, 1, 0)).squaredNorm();
    Float lc2 = (*this)(Vector3f(0, 0, 1)).squaredNorm();
#define NOT_ONE(x) ((x) < .999f || (x) > 1.001f)
    return (NOT_ONE(la2) || NOT_ONE(lb2) || NOT_ONE(lc2));
#undef NOT_ONE
  }

  template <ArithmeticType T>
  inline Point3<T> operator()(const Point3<T> &p) const;
  template <ArithmeticType T>
  inline Vector3<T> operator()(const Vector3<T> &v) const;
  template <ArithmeticType T>
  inline Normal3<T> operator()(const Normal3<T> &n) const;
  inline Ray operator()(const Ray &r) const;
  inline RayDifferential operator()(const RayDifferential &r) const;
  Transform operator*(const Transform &rhs) const;
  [[nodiscard]] bool swapsHandedNess() const;

 private:
  Matrix4x4 m, mInv;
};

template <ArithmeticType T>
inline Point3<T> Transform::operator()(const Point3<T> &p) const {
  Eigen::Vector4<T> v = m._data * Eigen::Vector4<T>(p.x(), p.y(), p.z(), 1);
  DAKKU_CHECK(v.w() != 0, "divide by zero");
  if (v.w() == 1) return Point3<T>(v.x(), v.y(), v.z());
  return Point3<T>(v.x(), v.y(), v.z()) / v.w();
}

template <ArithmeticType T>
inline Vector3<T> Transform::operator()(const Vector3<T> &v) const {
  return Vector3<T>(m._data.block<3, 3>(0, 0) * v._data);
}

template <ArithmeticType T>
inline Normal3<T> Transform::operator()(const Normal3<T> &n) const {
  return Normal3<T>(mInv._data.block<3, 3>(0, 0).transpose() * n._data);
}

inline Ray Transform::operator()(const Ray &r) const {
  Point3f o = (*this)(r.o);
  Vector3f d = (*this)(r.d);
  return Ray(o, d, r.tMax);
}

RayDifferential Transform::operator()(const RayDifferential &r) const {
  Ray tr = (*this)(static_cast<const Ray &>(r));
  RayDifferential ret(tr.o, tr.d, tr.tMax);
  ret.hasDifferentials = r.hasDifferentials;
  ret.rxOrigin = (*this)(r.rxOrigin);
  ret.ryOrigin = (*this)(r.ryOrigin);
  ret.rxDirection = (*this)(r.rxDirection);
  ret.ryDirection = (*this)(r.ryDirection);
  return ret;
}

Transform Transform::operator*(const Transform &rhs) const {
  return Transform(m * rhs.m, rhs.mInv * mInv);
}

bool Transform::swapsHandedNess() const {
  return m._data.block<3, 3>(0, 0).determinant() < 0;
}

DAKKU_END
#endif  // DAKKU_SRC_CORE_TRANSFORM_H_
