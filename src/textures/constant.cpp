#include <textures/constant.h>

namespace dakku {

ConstantTexture::ConstantTexture(float v) : value(v) { this->floatType = true; }

ConstantTexture::ConstantTexture(Spectrum v) : value(std::move(v)) {
  this->floatType = false;
}

ConstantTexture::ConstantTexture(const Property &p) : Texture(p) {
  if (p["value"].isNumberType()) {
    this->value = Spectrum(p["value"].getNumber());
  } else {
    this->value = p["value"].getVector();
  }
}

Spectrum ConstantTexture::evaluate(const SurfaceInteraction &) const { return this->value; }
}  // namespace dakku