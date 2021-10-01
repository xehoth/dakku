//
// Created by xehoth on 2021/10/1.
//
#include <dakku/core/transform.h>
#include <glm/gtc/matrix_transform.hpp>

namespace dakku {

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &rhs) const {
  return Matrix4x4(m * rhs.m);
}

Matrix4x4 inverse(const Matrix4x4 &m) { return Matrix4x4(glm::inverse(m.m)); }

Transform::Transform(const Matrix4x4 &m) : m(m), mInv(inverse(m)) {}

Transform::Transform(const Matrix4x4 &m, const Matrix4x4 &mInv) : m(m), mInv(mInv) {}

Transform inverse(const Transform &t) { return Transform(t.mInv, t.m); }

}