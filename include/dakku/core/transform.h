//
// Created by xehoth on 2021/10/1.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_TRANSFORM_H_
#define DAKKU_INCLUDE_DAKKU_CORE_TRANSFORM_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>

namespace dakku {

struct Matrix4x4 {
  Matrix4x4() : m(1) {}
  explicit Matrix4x4(const glm::mat<4, 4, Float> &m) : m(m) {}

  Matrix4x4 operator*(const Matrix4x4 &rhs) const;
  friend Matrix4x4 inverse(const Matrix4x4 &m);
  bool operator==(const Matrix4x4 &rhs) const;
  glm::mat<4, 4, Float> m;
};

class Transform {
 public:
  Transform() = default;
  explicit Transform(const Matrix4x4 &m);
  explicit Transform(const Matrix4x4 &m, const Matrix4x4 &mInv);

  Transform operator*(const Transform &t) const;
  Point3f operator()(const Point3f &p) const;
  Vector3f operator()(const Vector3f &p) const;
  Normal3f operator()(const Normal3f &n) const;
  Ray operator()(const Ray &r) const;

  friend Transform inverse(const Transform &t);
  [[nodiscard]] bool isIdentity() const;

 private:
  Matrix4x4 m, mInv;
};

Transform lookAt(const Point3f &eye, const Vector3f &look,
                 const Vector3f &up = Vector3f(0, 1, 0));
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_TRANSFORM_H_
