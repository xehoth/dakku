//
// Created by xehoth on 2021/10/4.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_TEXTURE_H_
#define DAKKU_INCLUDE_DAKKU_CORE_TEXTURE_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>

namespace dakku {

template <typename T>
class Texture {
 public:
  virtual T evaluate(const SurfaceInteraction &) const = 0;
  virtual ~Texture() = default;
};

class TextureMapping2D {
 public:
  virtual ~TextureMapping2D() = default;
  [[nodiscard]] virtual Point2f map(const SurfaceInteraction &si) const = 0;
};

class UVMapping2D : public TextureMapping2D {
 public:
  explicit UVMapping2D(Float su = 1, Float sv = 1, Float du = 0, Float dv = 0);
  [[nodiscard]] Point2f map(const SurfaceInteraction &si) const override;
 private:
  const Float su, sv, du, dv;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_TEXTURE_H_
