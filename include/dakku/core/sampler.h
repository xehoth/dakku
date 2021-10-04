//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_SAMPLER_H_
#define DAKKU_INCLUDE_DAKKU_CORE_SAMPLER_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>
#include <cstdint>

namespace dakku {

class Sampler {
 public:
  virtual ~Sampler() = default;
  explicit Sampler(std::uint32_t samplesPerPixel);
  virtual Float get1D() = 0;
  virtual Point2f get2D() = 0;
  CameraSample getCameraSample(const Point2i &pRaster);

  const std::uint32_t samplesPerPixel;
};
}
#endif  // DAKKU_INCLUDE_DAKKU_CORE_SAMPLER_H_
