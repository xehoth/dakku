#include <textures/constant.h>

namespace dakku {

ConstantTexture::ConstantTexture(float v) : value(v) { this->floatType = true; }

ConstantTexture::ConstantTexture(Spectrum v) : value(std::move(v)) {
  this->floatType = false;
}

Spectrum ConstantTexture::evaluate() const { return this->value; }
}  // namespace dakku