//
// Created by xehoth on 2021/10/7.
//

#ifndef DAKKU_INCLUDE_DAKKU_TEXTURES_IMAGEMAP_H_
#define DAKKU_INCLUDE_DAKKU_TEXTURES_IMAGEMAP_H_
#include <dakku/core/texture.h>
#include <dakku/core/image.h>
#include <dakku/core/interaction.h>
#include <map>
#include <string>
#include <memory>

namespace dakku {

struct TexInfo {
  explicit TexInfo(const std::string &fileName, ImageWrap wrapMode, bool gamma);
  bool operator<(const TexInfo &rhs) const;

  std::string fileName;
  ImageWrap wrapMode;
  bool gamma;
};

template <typename MemT, typename RetT>
class ImageTexture : public Texture<RetT> {
 public:
  ImageTexture(std::unique_ptr<TextureMapping2D> m, const std::string &fileName,
               ImageWrap wrapMode, bool gamma);
  static void clearCache();
  [[nodiscard]] RetT evaluate(const SurfaceInteraction &si) const override;

 private:
  static Image<MemT> *getTexture(const std::string &fileName,
                                 ImageWrap wrapMode, bool gamma);

  std::unique_ptr<TextureMapping2D> mapping;
  Image<MemT> *image;

  static std::map<TexInfo, std::unique_ptr<Image<MemT>>> textures;
};

template <typename MemT, typename RetT>
void ImageTexture<MemT, RetT>::clearCache() {}

template <typename MemT, typename RetT>
RetT ImageTexture<MemT, RetT>::evaluate(const SurfaceInteraction &si) const {
  Point2f st = mapping->map(si);
  MemT mem = image->lookUp(si.uv);
  return mem;
}

extern template class ImageTexture<Float, Float>;
extern template class ImageTexture<RGBSpectrum, RGBSpectrum>;

ImageTexture<Float, Float> *createImageFloatTexture(
    const std::string &fileName, Float uScale = 1, Float vScale = 1,
    Float uDelta = 0, Float vDelta = 1, ImageWrap wrapMode = ImageWrap::REPEAT);

ImageTexture<RGBSpectrum, RGBSpectrum> *createImageSpectrumTexture(
    const std::string &fileName, Float uScale = 1, Float vScale = 1,
    Float uDelta = 0, Float vDelta = 1, ImageWrap wrapMode = ImageWrap::REPEAT);

}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_TEXTURES_IMAGEMAP_H_
