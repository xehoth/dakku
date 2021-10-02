//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_INTEGRATORS_PATH_H_
#define DAKKU_INCLUDE_DAKKU_INTEGRATORS_PATH_H_
#include <dakku/core/integrator.h>

namespace dakku {

class PathIntegrator : public SamplerIntegrator {
 public:
  PathIntegrator(int maxDepth, std::shared_ptr<const Camera> camera,
                 std::shared_ptr<Sampler> sampler, Float rrThreshold = 1);
  void preprocess(const Scene &scene, Sampler &sampler) override;
  RGBSpectrum radiance(const Ray &ray, const Scene &scene,
                       Sampler &sampler) const override;

 private:
  const int maxDepth;
  const Float rrThreshold;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_INTEGRATORS_PATH_H_
