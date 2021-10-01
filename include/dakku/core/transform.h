//
// Created by xehoth on 2021/10/1.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_TRANSFORM_H_
#define DAKKU_INCLUDE_DAKKU_CORE_TRANSFORM_H_
#include "dakku.h"
#include "geometry.h"

namespace dakku {

struct Matrix4x4 {
  Matrix4x4() : m(1) {}
  explicit Matrix4x4(const glm::mat<4, 4, Float> &m) : m(m) {}

  Matrix4x4 operator*(const Matrix4x4 &rhs) const;
  friend Matrix4x4 inverse(const Matrix4x4 &m);

  glm::mat<4, 4, Float> m;
};

class Transform {
 public:
  Transform() = default;
  explicit Transform(const Matrix4x4 &m);
  explicit Transform(const Matrix4x4 &m, const Matrix4x4 &mInv);

  friend Transform inverse(const Transform &t);
 private:
  Matrix4x4 m, mInv;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_TRANSFORM_H_
