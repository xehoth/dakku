#ifndef DAKKU_SRC_SHAPES_API_H_
#define DAKKU_SRC_SHAPES_API_H_
#include <core/fwd.h>
#include <core/object.h>
#include <core/class.h>
#include <shapes/triangle.h>

DAKKU_BEGIN
namespace shapes {
void init() {
  DAKKU_REGISTER_CLASS(Triangle);
  DAKKU_REGISTER_CLASS(TriangleMesh);
}
}  // namespace shapes
DAKKU_END
#endif  // DAKKU_SRC_SHAPES_API_H_
