//
// Created by xehoth on 2021/10/7.
//

#ifndef DAKKU_INCLUDE_DAKKU_TEXTURES_IMAGEMAP_H_
#define DAKKU_INCLUDE_DAKKU_TEXTURES_IMAGEMAP_H_
#include <dakku/core/texture.h>
#include <string>
#include <memory>

namespace dakku {

struct TexInfo {
  explicit TexInfo(const std::string &fileName);
  bool operator<(const TexInfo &rhs) const;

  std::string fileName;
};

template <typename MemT, typename RetT>
class ImageTexture : public Texture<RetT> {
 public:
  ImageTexture(std::unique_ptr<TextureMapping2D> m,
               const std::string &fileName);
  static void clearCache();
  RetT evaluate(const SurfaceInteraction &si) const override;

 private:
  std::unique_ptr<TextureMapping2D> mapping;
};

template <typename MemT, typename RetT>
ImageTexture<MemT, RetT>::ImageTexture(std::unique_ptr<TextureMapping2D> m,
                                       const std::string &fileName) {}

template <typename MemT, typename RetT>
void ImageTexture<MemT, RetT>::clearCache() {
  // TODO:
}

template <typename MemT, typename RetT>
RetT ImageTexture<MemT, RetT>::evaluate(const SurfaceInteraction &si) const {
  Point2f st = mapping->map(si);
  // TODO: evaluate
}

}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_TEXTURES_IMAGEMAP_H_
