#ifndef DAKKU_TEXTURES_CONSTANT_H_
#define DAKKU_TEXTURES_CONSTANT_H_
#include <textures/fwd.h>
#include <core/spectrum.h>
namespace dakku {

class DAKKU_EXPORT_TEXTURES ConstantTexture : public Texture {
 public:
  DAKKU_DECLARE_OBJECT(ConstantTexture, Texture);

  explicit ConstantTexture(float v);
  explicit ConstantTexture(Spectrum v);
  [[nodiscard]] Spectrum evaluate() const override;

 private:
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  Spectrum value;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};
}  // namespace dakku
#endif