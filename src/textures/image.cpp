#include <textures/image.h>
#include <imageio/imageio.h>
#include <core/relative.h>

namespace dakku {

ImageTexture::ImageTexture(std::unique_ptr<TextureMapping2D> m,
                           const std::filesystem::path &fileName,
                           bool doTrilinear, float maxAnisotropy,
                           ImageWrapMode mode, float scale, bool gamma,
                           bool floatType)
    : Texture(floatType) {
  Point2i resolution;
  if (floatType) {
    std::unique_ptr<float[]> texels{
        readImage(RelativeRoot::instance().get(fileName), resolution.x(),
                  resolution.y(), true, static_cast<int>(gamma), scale)};
    if (!texels) {
      DAKKU_ERR("failed to load texture: {}", fileName.string());
    }
    mipmap = std::make_unique<MipMap<float>>(
        resolution,
        std::span{texels.get(),
                  static_cast<size_t>(resolution.x() * resolution.y())},
        doTrilinear, maxAnisotropy, mode);
  } else {
    std::unique_ptr<Spectrum[]> texels{reinterpret_cast<Spectrum *>(
        readImage(RelativeRoot::instance().get(fileName), resolution.x(),
                  resolution.y(), true, static_cast<int>(gamma), scale))};
    if (!texels) {
      DAKKU_ERR("failed to load texture: {}", fileName.string());
    }
    mipmap = std::make_unique<MipMap<Spectrum>>(
        resolution,
        std::span{texels.get(),
                  static_cast<size_t>(resolution.x() * resolution.y())},
        doTrilinear, maxAnisotropy, mode);
  }
}

std::unique_ptr<TextureMapping2D> createTextureMapping(const Property &p) {
  std::string type = "uv";
  if (p.getObject().contains("mapping")) {
  }
  std::unique_ptr<TextureMapping2D> ret{};
  if (type == "uv") {
    float su = p["mapping"].getNumberIf("su", 1.0f);
    float sv = p["mapping"].getNumberIf("sv", 1.0f);
    float du = p["mapping"].getNumberIf("du", 0.0f);
    float dv = p["mapping"].getNumberIf("dv", 0.0f);
    ret = std::make_unique<UvMapping2D>(su, sv, du, dv);
  }
  return ret;
}

ImageWrapMode getImageWrapMode(const Property &p) {
  ImageWrapMode ret = ImageWrapMode::REPEAT;
  if (p.getObject().contains("mode")) {
    std::string mode = p["mode"].getString();
    if (mode == "clamp") {
      ret = ImageWrapMode::CLAMP;
    } else if (mode == "black") {
      ret = ImageWrapMode::BLACK;
    }
  }
  return ret;
}

ImageTexture::ImageTexture(const Property &p)
    : ImageTexture(createTextureMapping(p), p["fileName"].getString(),
                   p.getNumberIf("doTrilinear", true),
                   p.getNumberIf("maxAnisotropy", 8.0f), getImageWrapMode(p),
                   p.getNumberIf("scale", 1.0f), p.getNumberIf("gamma", true),
                   p.getNumberIf("floatType", false)) {}

Spectrum ImageTexture::evaluate(const SurfaceInteraction &si) const {
  Vector2f dstdx, dstdy;
  Point2f st = mapping->map(si, dstdx, dstdy);
  return floatType
             ? Spectrum{std::get<std::unique_ptr<MipMap<float>>>(mipmap)
                            ->lookup(st, dstdx, dstdy)}
             : std::get<std::unique_ptr<MipMap<Spectrum>>>(mipmap)->lookup(
                   st, dstdx, dstdy);
}
}  // namespace dakku