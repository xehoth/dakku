#include <textures/constant.h>
DAKKU_BEGIN

void ConstantFloatTexture::serialize(Json &json, OutputStream *) const {
  json["class"] = getClassNameStatic();
  DAKKU_SER_J(data);
}

void ConstantFloatTexture::unserialize(const Json &json, InputStream *) {
  DAKKU_UNSER_JI(data);
}

void ConstantSpectrumTexture::serialize(Json &json, OutputStream *) const {
  json["class"] = getClassNameStatic();
  DAKKU_SER_J(data);
}

void ConstantSpectrumTexture::unserialize(const Json &json, InputStream *) {
  DAKKU_UNSER_J(data);
}

DAKKU_END
