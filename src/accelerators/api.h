#ifndef DAKKU_SRC_ACCELERATORS_API_H_
#define DAKKU_SRC_ACCELERATORS_API_H_
#include <core/class.h>
#include <accelerators/embree_accel.h>

DAKKU_BEGIN
namespace accelerators {
void init() { DAKKU_REGISTER_CLASS(EmbreeAccel); }
}  // namespace accelerators
DAKKU_END
#endif  // DAKKU_SRC_ACCELERATORS_API_H_
