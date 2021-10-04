//
// Created by xehoth on 2021/10/2.
//
#include <dakku/samplers/random.h>

namespace dakku {

Float RandomSampler::get1D() {
  std::uniform_real_distribution<Float> dis(0, 1);
  return dis(engine);
}

Point2f RandomSampler::get2D() { return Point2f{get1D(), get1D()}; }

RandomSampler::RandomSampler(std::uint32_t spp) : Sampler(spp) {}
}  // namespace dakku