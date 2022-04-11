#ifndef DAKKU_TEXTURES_IMAGE_H_
#define DAKKU_TEXTURES_IMAGE_H_
#include <textures/fwd.h>
#include <core/mipmap.h>
#include <filesystem>

namespace dakku {

class DAKKU_EXPORT_TEXTURES ImageTexture : public Texture {
 public:
  DAKKU_DECLARE_OBJECT(ImageTexture, Texture);
  explicit ImageTexture(std::unique_ptr<TextureMapping2D> m,
                        const std::filesystem::path &fileName, bool doTrilinear,
                        float maxAnisotropy, ImageWrapMode mode, float scale,
                        bool gamma, bool floatType = false);
  [[nodiscard]] Spectrum evaluate(const SurfaceInteraction &si) const override;

 private:
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  std::unique_ptr<TextureMapping2D> mapping;
  std::variant<std::unique_ptr<MipMap<float>>,
               std::unique_ptr<MipMap<Spectrum>>>
      mipmap;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};
}  // namespace dakku
#endif