//
// Created by xehoth on 2021/10/7.
//
#include <dakku/textures/imagemap.h>

namespace dakku {

TexInfo::TexInfo(const std::string &fileName, ImageWrap wrapMode, bool gamma)
    : fileName(fileName), wrapMode(wrapMode), gamma(gamma) {}

bool TexInfo::operator<(const TexInfo &rhs) const {
  if (fileName != rhs.fileName) return fileName < rhs.fileName;
  if (gamma != rhs.gamma) return !gamma;
  return wrapMode < rhs.wrapMode;
}

template <typename MemT, typename RetT>
ImageTexture<MemT, RetT>::ImageTexture(std::unique_ptr<TextureMapping2D> m,
                                       const std::string &fileName,
                                       ImageWrap wrapMode, bool gamma)
    : mapping(std::move(m)) {
  image = getTexture(fileName, wrapMode, gamma);
}
template <typename MemT, typename RetT>
Image<MemT> *ImageTexture<MemT, RetT>::getTexture(const std::string &fileName,
                                                  ImageWrap wrapMode,
                                                  bool gamma) {
  TexInfo texInfo(fileName, wrapMode, gamma);
  if (auto it = textures.find(texInfo); it != textures.end())
    return it->second.get();
  Point2i resolution;
  //  TODO: load image

}

ImageTexture<Float, Float> *createImageFloatTexture(const std::string &fileName,
                                                    Float uScale, Float vScale,
                                                    Float uDelta, Float vDelta,
                                                    ImageWrap wrapMode) {
  std::unique_ptr<TextureMapping2D> map =
      std::make_unique<UVMapping2D>(uScale, vScale, uDelta, vDelta);
  bool gamma = fileName.ends_with(".png");
  return new ImageTexture<Float, Float>(std::move(map), fileName, wrapMode,
                                        gamma);
}

ImageTexture<RGBSpectrum, RGBSpectrum> *createImageSpectrumTexture(
    const std::string &fileName, Float uScale, Float vScale, Float uDelta,
    Float vDelta, ImageWrap wrapMode) {
  std::unique_ptr<TextureMapping2D> map =
      std::make_unique<UVMapping2D>(uScale, vScale, uDelta, vDelta);
  bool gamma = fileName.ends_with(".png");
  return new ImageTexture<RGBSpectrum, RGBSpectrum>(std::move(map), fileName,
                                                    wrapMode, gamma);
}

}  // namespace dakku