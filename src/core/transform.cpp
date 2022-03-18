#include <core/transform.h>

DAKKU_BEGIN

Matrix4x4::Matrix4x4(const Float m[4][4]) {
  _data << m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[1][2],
      m[1][3], m[2][0], m[2][1], m[2][2], m[2][3], m[3][0], m[3][1], m[3][2],
      m[3][3];
}
Matrix4x4::Matrix4x4(Float t00, Float t01, Float t02, Float t03, Float t10,
                     Float t11, Float t12, Float t13, Float t20, Float t21,
                     Float t22, Float t23, Float t30, Float t31, Float t32,
                     Float t33) {
  _data << t00, t01, t02, t03, t10, t11, t12, t13, t20, t21, t22, t23, t30, t31,
      t32, t33;
}

Matrix4x4 transpose(const Matrix4x4 &mat) {
  return Matrix4x4(mat._data.transpose());
}
Matrix4x4 inverse(const Matrix4x4 &mat) {
  return Matrix4x4(mat._data.inverse());
}

DAKKU_END