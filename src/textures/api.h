#ifndef DAKKU_SRC_TEXTURES_API_H_
#define DAKKU_SRC_TEXTURES_API_H_
#include <core/class.h>
#include <textures/constant.h>

DAKKU_BEGIN
namespace textures {
void init() {
  DAKKU_REGISTER_CLASS(ConstantFloatTexture);
  DAKKU_REGISTER_CLASS(ConstantSpectrumTexture);
}
}  // namespace textures
DAKKU_END
#endif  // DAKKU_SRC_TEXTURES_API_H_
