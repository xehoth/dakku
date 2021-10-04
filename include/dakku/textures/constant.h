//
// Created by xehoth on 2021/10/4.
//

#ifndef DAKKU_INCLUDE_DAKKU_TEXTURES_CONSTANT_H_
#define DAKKU_INCLUDE_DAKKU_TEXTURES_CONSTANT_H_
#include <dakku/core/texture.h>

namespace dakku {

template <typename T>
class ConstantTexture : public Texture<T> {
 public:
  explicit ConstantTexture(const T &value) : value(value) {}
  T evaluate(const SurfaceInteraction &) const override { return value; }
 private:
  const T value;
};

}
#endif  // DAKKU_INCLUDE_DAKKU_TEXTURES_CONSTANT_H_
