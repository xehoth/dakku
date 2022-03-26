#ifndef DAKKU_SRC_TEXTURES_CONSTANT_H_
#define DAKKU_SRC_TEXTURES_CONSTANT_H_
#include <core/texture.h>
#include <core/spectrum.h>

DAKKU_BEGIN
template <TextureDataType T>
class ConstantTexture : public Texture<T> {
 public:
  DAKKU_DECLARE_OBJECT(ConstantTexture);

  void construct(const T &_data) { this->data = _data; }
  [[nodiscard]] T evaluate(const SurfaceInteraction &) const override { return data; }

 protected:
  T data{1};
};

class ConstantFloatTexture : public ConstantTexture<Float> {
 public:
  DAKKU_DECLARE_OBJECT(ConstantFloatTexture);

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;
};

class ConstantSpectrumTexture : public ConstantTexture<Spectrum> {
 public:
  DAKKU_DECLARE_OBJECT(ConstantSpectrumTexture);

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;
};
DAKKU_END

#endif  // DAKKU_SRC_TEXTURES_CONSTANT_H_
