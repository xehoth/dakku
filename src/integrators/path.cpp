//
// Created by xehoth on 2021/10/2.
//
#include <dakku/integrators/path.h>
#include <dakku/core/spectrum.h>
#include <dakku/core/interaction.h>
#include <dakku/core/scene.h>

namespace dakku {

PathIntegrator::PathIntegrator(int maxDepth,
                               std::shared_ptr<const Camera> camera,
                               std::shared_ptr<Sampler> sampler,
                               Float rrThreshold)
    : SamplerIntegrator(camera, sampler),
      maxDepth(maxDepth),
      rrThreshold(rrThreshold) {}

void PathIntegrator::preprocess(const Scene &scene, Sampler &sampler) {}

RGBSpectrum PathIntegrator::radiance(const Ray &r, const Scene &scene,
                                     Sampler &sampler) const {
  Ray ray(r);
  RGBSpectrum L(0), beta(1);
  bool specularBounce = false;
  int bounces = 0;

  Float etaScale = 1;

  for (;; ++bounces) {
    SurfaceInteraction isect;
    bool foundInteraction = scene.intersect(ray, isect);

    if (bounces == 0 || specularBounce) {
      if (foundInteraction) {
        L += beta * isect.emit(-ray.d);
      } else {
        // TODO: Infinite lights
      }
    }
    if (!foundInteraction || bounces >= maxDepth) break;

    isect.computeScatteringFunctions(ray);
  }
  return L;
}

}  // namespace dakku