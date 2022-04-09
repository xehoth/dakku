#ifndef DAKKU_TEXTURES_CONSTANT_H_
#define DAKKU_TEXTURES_CONSTANT_H_
#include <textures/fwd.h>

namespace dakku {

class DAKKU_EXPORT_TEXTURES ContantTexture : public Texture {
 public:
  [[nodiscard]] Spectrum evaluate() const override;
};
}  // namespace dakku
#endif