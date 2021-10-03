//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_INTEGRATOR_H_
#define DAKKU_INCLUDE_DAKKU_CORE_INTEGRATOR_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>
#include <memory>
namespace dakku {

class Integrator {
 public:
  virtual ~Integrator() = default;
  virtual void render(const Scene &scene) = 0;
};

class SamplerIntegrator : public Integrator {
 public:
  explicit SamplerIntegrator(std::shared_ptr<const Camera> camera,
                             std::shared_ptr<Sampler> sampler);
  virtual void preprocess(const Scene &scene, Sampler &sampler);
  void render(const Scene &scene) override;
  virtual RGBSpectrum radiance(const Ray &ray, const Scene &scene,
                               Sampler &sampler) const = 0;

 protected:
  std::shared_ptr<const Camera> camera{nullptr};

 private:
  std::shared_ptr<Sampler> sampler;
};

RGBSpectrum uniformSampleOneLight(const Interaction &it, const Scene &scene,
                                  Sampler &sampler);
RGBSpectrum estimateDirect(const Interaction &it, const Point2f &uShading,
                           const Light &light, const Point2f &uLight,
                           const Scene &scene, Sampler &sampler);
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_INTEGRATOR_H_
