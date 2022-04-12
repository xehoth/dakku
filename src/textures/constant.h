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
  explicit ConstantTexture(const Property &);
  [[nodiscard]] Spectrum evaluate(const SurfaceInteraction &) const override;

  Spectrum value;
};
}  // namespace dakku
#endif