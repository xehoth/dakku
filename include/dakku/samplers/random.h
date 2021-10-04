//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_SAMPLERS_RANDOM_H_
#define DAKKU_INCLUDE_DAKKU_SAMPLERS_RANDOM_H_
#include <dakku/core/sampler.h>
#include <random>

namespace dakku {

class RandomSampler : public Sampler {
 public:
  explicit RandomSampler(std::uint32_t spp);
  Float get1D() override;
  Point2f get2D() override;
 private:
  std::default_random_engine engine;
};
}
#endif  // DAKKU_INCLUDE_DAKKU_SAMPLERS_RANDOM_H_
