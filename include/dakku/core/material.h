//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_MATERIAL_H_
#define DAKKU_INCLUDE_DAKKU_CORE_MATERIAL_H_
#include <dakku/core/dakku.h>

namespace dakku {

class Material {
 public:
  virtual void computeScatteringFunctions(SurfaceInteraction &si) const = 0;
  virtual ~Material() = default;
};
}
#endif  // DAKKU_INCLUDE_DAKKU_CORE_MATERIAL_H_
