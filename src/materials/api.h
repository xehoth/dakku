#ifndef DAKKU_SRC_MATERIALS_API_H_
#define DAKKU_SRC_MATERIALS_API_H_
#include <core/class.h>
#include <materials/matte.h>

DAKKU_BEGIN
namespace materials {
void init() {
  DAKKU_REGISTER_CLASS(MatteMaterial);
}
}
DAKKU_END
#endif  // DAKKU_SRC_MATERIALS_API_H_
