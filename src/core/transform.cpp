#include <core/transform.h>

namespace dakku {

Matrix4x4 transpose(const Matrix4x4 &m) {
  return {m[0][0], m[1][0], m[2][0], m[3][0], m[0][1], m[1][1],
          m[2][1], m[3][1], m[0][2], m[1][2], m[2][2], m[3][2],
          m[0][3], m[1][3], m[2][3], m[3][3]};
}

Matrix4x4 Matrix4x4::mul(const Matrix4x4 &m1, const Matrix4x4 &m2) {
  Matrix4x4 c;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      c.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] +
                  m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
  return c;
}

Matrix4x4 inverse(const Matrix4x4 &m) {
  Matrix4x4 inv;
  inv[0][0] = m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[3][2] * m[2][3] -
              m[1][2] * m[2][1] * m[3][3] + m[1][2] * m[3][1] * m[2][3] +
              m[1][3] * m[2][1] * m[3][2] - m[1][3] * m[3][1] * m[2][2];

  inv[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[3][2] * m[2][3] +
              m[0][2] * m[2][1] * m[3][3] - m[0][2] * m[3][1] * m[2][3] -
              m[0][3] * m[2][1] * m[3][2] + m[0][3] * m[3][1] * m[2][2];

  inv[0][2] = m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[3][2] * m[1][3] -
              m[0][2] * m[1][1] * m[3][3] + m[0][2] * m[3][1] * m[1][3] +
              m[0][3] * m[1][1] * m[3][2] - m[0][3] * m[3][1] * m[1][2];

  inv[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[2][2] * m[1][3] +
              m[0][2] * m[1][1] * m[2][3] - m[0][2] * m[2][1] * m[1][3] -
              m[0][3] * m[1][1] * m[2][2] + m[0][3] * m[2][1] * m[1][2];

  inv[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[3][2] * m[2][3] +
              m[1][2] * m[2][0] * m[3][3] - m[1][2] * m[3][0] * m[2][3] -
              m[1][3] * m[2][0] * m[3][2] + m[1][3] * m[3][0] * m[2][2];

  inv[1][1] = m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[3][2] * m[2][3] -
              m[0][2] * m[2][0] * m[3][3] + m[0][2] * m[3][0] * m[2][3] +
              m[0][3] * m[2][0] * m[3][2] - m[0][3] * m[3][0] * m[2][2];

  inv[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[3][2] * m[1][3] +
              m[0][2] * m[1][0] * m[3][3] - m[0][2] * m[3][0] * m[1][3] -
              m[0][3] * m[1][0] * m[3][2] + m[0][3] * m[3][0] * m[1][2];

  inv[1][3] = m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[2][2] * m[1][3] -
              m[0][2] * m[1][0] * m[2][3] + m[0][2] * m[2][0] * m[1][3] +
              m[0][3] * m[1][0] * m[2][2] - m[0][3] * m[2][0] * m[1][2];

  inv[2][0] = m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[3][1] * m[2][3] -
              m[1][1] * m[2][0] * m[3][3] + m[1][1] * m[3][0] * m[2][3] +
              m[1][3] * m[2][0] * m[3][1] - m[1][3] * m[3][0] * m[2][1];

  inv[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[3][1] * m[2][3] +
              m[0][1] * m[2][0] * m[3][3] - m[0][1] * m[3][0] * m[2][3] -
              m[0][3] * m[2][0] * m[3][1] + m[0][3] * m[3][0] * m[2][1];

  inv[2][2] = m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[3][1] * m[1][3] -
              m[0][1] * m[1][0] * m[3][3] + m[0][1] * m[3][0] * m[1][3] +
              m[0][3] * m[1][0] * m[3][1] - m[0][3] * m[3][0] * m[1][1];

  inv[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[2][1] * m[1][3] +
              m[0][1] * m[1][0] * m[2][3] - m[0][1] * m[2][0] * m[1][3] -
              m[0][3] * m[1][0] * m[2][1] + m[0][3] * m[2][0] * m[1][1];

  inv[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[3][1] * m[2][2] +
              m[1][1] * m[2][0] * m[3][2] - m[1][1] * m[3][0] * m[2][2] -
              m[1][2] * m[2][0] * m[3][1] + m[1][2] * m[3][0] * m[2][1];

  inv[3][1] = m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[3][1] * m[2][2] -
              m[0][1] * m[2][0] * m[3][2] + m[0][1] * m[3][0] * m[2][2] +
              m[0][2] * m[2][0] * m[3][1] - m[0][2] * m[3][0] * m[2][1];

  inv[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[3][1] * m[1][2] +
              m[0][1] * m[1][0] * m[3][2] - m[0][1] * m[3][0] * m[1][2] -
              m[0][2] * m[1][0] * m[3][1] + m[0][2] * m[3][0] * m[1][1];

  inv[3][3] = m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[2][1] * m[1][2] -
              m[0][1] * m[1][0] * m[2][2] + m[0][1] * m[2][0] * m[1][2] +
              m[0][2] * m[1][0] * m[2][1] - m[0][2] * m[2][0] * m[1][1];

  float det = m[0][0] * inv[0][0] + m[1][0] * inv[0][1] + m[2][0] * inv[0][2] +
              m[3][0] * inv[0][3];

  if (det == 0) {
    DAKKU_ERR("singular matrix");
  }
  det = 1 / det;

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) inv[i][j] *= det;
  return inv;
}

Transform translate(const Vector3f &delta) {
  Matrix4x4 m(1, 0, 0, delta.x(), 0, 1, 0, delta.y(), 0, 0, 1, delta.z(), 0, 0,
              0, 1);
  Matrix4x4 mInv(1, 0, 0, -delta.x(), 0, 1, 0, -delta.y(), 0, 0, 1, -delta.z(),
                 0, 0, 0, 1);
  return {m, mInv};
}

Transform scale(float x, float y, float z) {
  Matrix4x4 m(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
  Matrix4x4 mInv(1 / x, 0, 0, 0, 0, 1 / y, 0, 0, 0, 0, 1 / z, 0, 0, 0, 0, 1);
  return {m, mInv};
}

Transform rotateX(float theta) {
  float sinTheta = std::sin(radians(theta));
  float cosTheta = std::cos(radians(theta));
  Matrix4x4 m(1, 0, 0, 0, 0, cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0,
              0, 0, 0, 1);
  return Transform{m, transpose(m)};
}

Transform rotateY(float theta) {
  float sinTheta = std::sin(radians(theta));
  float cosTheta = std::cos(radians(theta));
  Matrix4x4 m{cosTheta,  0, sinTheta, 0, 0, 1, 0, 0,
              -sinTheta, 0, cosTheta, 0, 0, 0, 0, 1};
  return Transform{m, transpose(m)};
}

Transform rotateZ(float theta) {
  float sinTheta = std::sin(radians(theta));
  float cosTheta = std::cos(radians(theta));
  Matrix4x4 m(cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0, 0, 0, 0, 1, 0,
              0, 0, 0, 1);
  return Transform{m, transpose(m)};
}

Transform rotate(float theta, const Vector3f &axis) {
  Vector3f a = axis.normalized();
  float sinTheta = std::sin(radians(theta));
  float cosTheta = std::cos(radians(theta));
  Matrix4x4 m;
  // compute rotation of first basis vector
  m(0, 0) = a.x() * a.x() + (1 - a.x() * a.x()) * cosTheta;
  m(0, 1) = a.x() * a.y() * (1 - cosTheta) - a.z() * sinTheta;
  m(0, 2) = a.x() * a.z() * (1 - cosTheta) + a.y() * sinTheta;
  m(0, 3) = 0;

  // compute rotations of second and third basis vectors
  m(1, 0) = a.x() * a.y() * (1 - cosTheta) + a.z() * sinTheta;
  m(1, 1) = a.y() * a.y() + (1 - a.y() * a.y()) * cosTheta;
  m(1, 2) = a.y() * a.z() * (1 - cosTheta) - a.x() * sinTheta;
  m(1, 3) = 0;

  m(2, 0) = a.x() * a.z() * (1 - cosTheta) - a.y() * sinTheta;
  m(2, 1) = a.y() * a.z() * (1 - cosTheta) + a.x() * sinTheta;
  m(2, 2) = a.z() * a.z() + (1 - a.z() * a.z()) * cosTheta;
  m(2, 3) = 0;
  return Transform{m, transpose(m)};
}

Transform lookAt(const Point3f &pos, const Point3f &look,
                 const Vector3f &refUp) {
  Matrix4x4 cameraToWorld;
  // translation
  cameraToWorld(0, 3) = pos.x();
  cameraToWorld(1, 3) = pos.y();
  cameraToWorld(2, 3) = pos.z();
  cameraToWorld(3, 3) = 1;

  // change of basis
  Vector3f dir = (look - pos).normalized();
  Vector3f right = refUp.cross(dir);
  if (right.norm() == 0) {
    DAKKU_ERR(
        "\"up\" vector {} and viewing direction {} "
        "passed to LookAt are pointing in the same direction.  Using "
        "the identity transformation.",
        refUp, dir);
    return {};
  }
  right.normalize();
  Vector3f up = dir.cross(right);
  cameraToWorld(0, 0) = right.x();
  cameraToWorld(1, 0) = right.y();
  cameraToWorld(2, 0) = right.z();
  cameraToWorld(3, 0) = 0;
  cameraToWorld(0, 1) = up.x();
  cameraToWorld(1, 1) = up.y();
  cameraToWorld(2, 1) = up.z();
  cameraToWorld(3, 1) = 0;
  cameraToWorld(0, 2) = dir.x();
  cameraToWorld(1, 2) = dir.y();
  cameraToWorld(2, 2) = dir.z();
  cameraToWorld(3, 2) = 0;
  return Transform{inverse(cameraToWorld), cameraToWorld};
}

Transform perspective(float fov, float n, float f) {
  // perform projective divide for perspective projection
  Matrix4x4 persp(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, f / (f - n), -f * n / (f - n),
                  0, 0, 1, 0);

  // scale canonical perspective view to specified field of view
  float invTanAng = 1 / std::tan(radians(fov) / 2);
  return scale(invTanAng, invTanAng, 1) * Transform(persp);
}
}  // namespace dakku