#ifndef DAKKU_SRC_CORE_TEXTURE_H_
#define DAKKU_SRC_CORE_TEXTURE_H_
#include <core/sobject.h>

DAKKU_BEGIN
template <TextureDataType T>
class Texture : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Texture);

  [[nodiscard]] virtual T evaluate(const SurfaceInteraction &) const = 0;

  static constexpr bool isFloatTexture() { return std::is_same_v<T, Float>; }
  static constexpr bool isSpectrumTexture() {
    return std::is_same_v<T, Spectrum>;
  }
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_TEXTURE_H_
