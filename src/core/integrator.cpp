//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/integrator.h>
#include <dakku/core/scene.h>
#include <dakku/core/camera.h>
#include <dakku/core/film.h>
#include <dakku/core/sampler.h>
#include <dakku/core/light.h>
#include <dakku/core/reflection.h>

namespace dakku {

SamplerIntegrator::SamplerIntegrator(std::shared_ptr<const Camera> camera,
                                     std::shared_ptr<Sampler> sampler)
    : camera(camera), sampler(sampler) {}

void SamplerIntegrator::preprocess(const Scene &scene, Sampler &sampler) {}

void SamplerIntegrator::render(const Scene &scene) {
  preprocess(scene, *sampler);
  const Point2i resolution = this->camera->film->fullResolution;
  constexpr int sqrtSpp = 8;
  constexpr int spp = sqrtSpp * sqrtSpp;
  constexpr Float invSqrtSpp = 1 / static_cast<Float>(sqrtSpp);
  constexpr Float invSpp = 1 / static_cast<Float>(spp);
  const Float totalSteps = static_cast<Float>(resolution.x);
  Float progress = 0;
//#pragma omp parallel for collapse(2) schedule(dynamic)
#pragma omp parallel for schedule(dynamic) num_threads(16)
  for (int x = 0; x < resolution.x; ++x) {
    printf("\r%04.2f%%", ++progress / totalSteps * 100);
    for (int y = 0; y < resolution.y; ++y) {
      RGBSpectrum res(0);
      for (int sx = 0; sx < sqrtSpp; ++sx) {
        for (int sy = 0; sy < sqrtSpp; ++sy) {
          Point2f uv = sampler->get2D();
          //          CameraSample cameraSample =
          //          sampler->getCameraSample(Point2i{x, y});
          CameraSample cameraSample;
          cameraSample.pFilm =
              Point2f{static_cast<Float>(x) +
                          (static_cast<Float>(sx) + uv.x) * invSqrtSpp,
                      static_cast<Float>(y) +
                          (static_cast<Float>(sy) + uv.y) * invSqrtSpp};
          Ray ray;
          camera->generateRay(cameraSample, ray);
          res += radiance(ray, scene, *sampler);
        }
      }
      camera->film->getPixel(Point2i{x, y}) = res * invSpp;
    }
  }
}

RGBSpectrum uniformSampleOneLight(const Interaction &it, const Scene &scene,
                                  Sampler &sampler) {
  int nLights = static_cast<int>(scene.lights.size());
  if (nLights == 0) return RGBSpectrum{0};
  int lightNum =
      std::min(static_cast<int>(sampler.get1D() * static_cast<Float>(nLights)),
               nLights - 1);
  Float lightPdf = static_cast<Float>(1) / static_cast<Float>(nLights);
  const std::shared_ptr<Light> &light = scene.lights[lightNum];
  Point2f uLight = sampler.get2D();
  Point2f uScattering = sampler.get2D();
  return estimateDirect(it, uScattering, *light, uLight, scene, sampler) /
         lightPdf;
}

RGBSpectrum estimateDirect(const Interaction &it, const Point2f &uShading,
                           const Light &light, const Point2f &uLight,
                           const Scene &scene, Sampler &sampler) {
  RGBSpectrum Ld(0);
  Vector3f wi;
  Float lightPdf = 0, scatteringPdf = 0;
  VisibilityTester visibility;
  RGBSpectrum Li = light.sampleLi(it, uLight, wi, lightPdf, visibility);

  if (lightPdf > 0 && !Li.isBlack()) {
    RGBSpectrum f;
    if (it.isSurfaceInteraction()) {
      const auto &isect = static_cast<const SurfaceInteraction &>(it);
      f = isect.bsdf.f(isect.wo, wi) * absDot(wi, isect.n);
      scatteringPdf = isect.bsdf.pdf(isect.wo, wi);
    } else {
      // TODO: volume
    }
    if (!f.isBlack()) {
      if (false) {
        // TODO: volume
      } else {
        if (!visibility.unoccluded(scene)) {
          Li = RGBSpectrum(0);
        }
      }
      if (!Li.isBlack()) {
        if (isDeltaLight(light.flags)) {
          Ld += f * Li / lightPdf;
        } else {
          // TODO: MIS
          Ld += f * Li / lightPdf;
        }
      }
    }
  }
  // TODO: BSDF sample, MIS
  return Ld;
}
}  // namespace dakku