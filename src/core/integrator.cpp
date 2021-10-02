//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/integrator.h>
#include <dakku/core/scene.h>
#include <dakku/core/camera.h>
#include <dakku/core/film.h>
#include <dakku/core/sampler.h>

namespace dakku {

SamplerIntegrator::SamplerIntegrator(std::shared_ptr<const Camera> camera,
                                     std::shared_ptr<Sampler> sampler)
    : camera(camera), sampler(sampler) {}

void SamplerIntegrator::preprocess(const Scene &scene, Sampler &sampler) {}

void SamplerIntegrator::render(const Scene &scene) {
  preprocess(scene, *sampler);
  const Point2i resolution = this->camera->film->fullResolution;
  for (int x = 0; x < resolution.x; ++x) {
    for (int y = 0; y < resolution.y; ++y) {
      CameraSample cameraSample = sampler->getCameraSample(Point2i{x, y});
      Ray ray;
      camera->generateRay(cameraSample, ray);
      RGBSpectrum res = radiance(ray, scene, *sampler);
      camera->film->getPixel(Point2i{x, y}) = res;
    }
  }
}

}  // namespace dakku