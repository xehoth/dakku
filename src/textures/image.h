#ifndef DAKKU_TEXTURES_IMAGE_H_
#define DAKKU_TEXTURES_IMAGE_H_
#include <textures/fwd.h>
#include <core/mipmap.h>
#include <filesystem>

namespace dakku {

class ImageTexture : public Texture {
 public:
  DAKKU_DECLARE_OBJECT(ImageTexture, Texture);
  explicit ImageTexture(std::unique_ptr<TextureMapping2D> m,
                        const std::filesystem::path &fileName, bool doTrilinear,
                        float maxAnisotropy, ImageWrapMode mode, float scale,
                        bool gamma, bool floatType = false);
  [[nodiscard]] Spectrum evaluate(const SurfaceInteraction &si) const override;

 private:
  std::unique_ptr<TextureMapping2D> mapping;
  std::variant<std::unique_ptr<MipMap<float>>,
               std::unique_ptr<MipMap<Spectrum>>>
      mipmap;
};
}  // namespace dakku
#endif