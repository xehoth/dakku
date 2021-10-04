//
// Created by xehoth on 2021/10/4.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_TEXTURE_H_
#define DAKKU_INCLUDE_DAKKU_CORE_TEXTURE_H_
#include <dakku/core/dakku.h>

namespace dakku {

template <typename T>
class Texture {
 public:
  virtual T evaluate(const SurfaceInteraction &) const = 0;
  virtual ~Texture() = default;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_TEXTURE_H_
