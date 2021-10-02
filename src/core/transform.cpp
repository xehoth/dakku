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

bool Matrix4x4::operator==(const Matrix4x4 &rhs) const {
  return this->m == rhs.m;
}

Transform::Transform(const Matrix4x4 &m) : m(m), mInv(inverse(m)) {}

Transform::Transform(const Matrix4x4 &m, const Matrix4x4 &mInv)
    : m(m), mInv(mInv) {}

Transform inverse(const Transform &t) { return Transform(t.mInv, t.m); }

Transform Transform::operator*(const Transform &t) const {
  return Transform(m * t.m, t.mInv * mInv);
}

bool Transform::isIdentity() const { return m == Matrix4x4(); }

Point3f Transform::operator()(const Point3f &p) const {
  using vec4 = glm::vec<4, Float>;
  using vec3 = glm::vec<3, Float>;
  vec4 res = m.m * vec4(p.v, 1);
  vec3 ret = vec3(res) / res.w;
  return Point3f{ret};
}

Vector3f Transform::operator()(const Vector3f &p) const {
  return Vector3f{glm::mat<3, 3, Float>(m.m) * p.v};
}

Ray Transform::operator()(const Ray &r) const {
  return Ray{(*this)(r.o), (*this)(r.d), r.tMax};
}

}  // namespace dakku