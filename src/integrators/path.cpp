#include <integrators/path.h>
#include <core/spectrum.h>
#include <core/ray.h>
#include <core/interaction.h>
#include <core/scene.h>
#include <core/reflection.h>
#include <core/sampler.h>
#include <core/state.h>

DAKKU_BEGIN
void PathIntegrator::construct(int _maxDepth, const Camera *camera,
                               Sampler *sampler, const Bounds2i &pixelBounds,
                               Float _rrThreshold) {
  SamplerIntegrator::construct(camera, sampler, pixelBounds);
  this->maxDepth = _maxDepth;
  this->rrThreshold = _rrThreshold;
}

void PathIntegrator::preprocess(const Scene &scene, Sampler &sampler) {
  SamplerIntegrator::preprocess(scene, sampler);
}

Spectrum PathIntegrator::radiance(const Ray &r, const Scene &scene,
                                  Sampler &sampler, MemoryArena &arena,
                                  int depth) const {
  Spectrum L(0), beta(1);
  Ray ray(r);
  bool specularBounce = false;
  int bounces = 0;

  Float etaScale = 1;

  for (bounces = 0;; ++bounces) {
    // intersect ray with scene and store intersection in isect
    SurfaceInteraction isect;
    bool foundIntersection = scene.intersect(ray, isect);

    // possibly add emitted light at intersection
    if (bounces == 0 || specularBounce) {
      // add emitted light at path vertex or from the environment
      if (foundIntersection) {
        L += beta * isect.emit(-ray.d);
      } else {
        // TODO: infinite lights
      }
    }
    // terminate path if ray escaped or _maxDepth_ was reached
    if (!foundIntersection || bounces >= maxDepth) break;
    // compute scattering functions and skip over medium boundaries
    isect.computeScatteringFunctions(ray, arena);
    if (!isect.bsdf) {
      DAKKU_DEBUG("skipping intersection due to null bsdf");
      ray = isect.spawnRay(ray.d);
      bounces--;
      continue;
    }
    // sample illumination from lights to find path contribution.
    // (but skip this for perfectly specular BSDFs.)
    if (isect.bsdf->numComponents(BxDFType::BSDF_ALL &
                                  static_cast<BxDFType>(~static_cast<int>(
                                      BxDFType::BSDF_SPECULAR))) > 0) {
      Spectrum Ld = beta * uniformSampleOneLight(isect, scene, arena, sampler);
      DAKKU_CHECK(Ld.y() > 0, "Ld.y() <= 0");
      L += Ld;
    }

    // sample BSDF to get new path direction
    Vector3f wo = -ray.d, wi;
    Float pdf = 0;
    BxDFType flags;
    Spectrum f = isect.bsdf->sampleF(wo, wi, sampler.get2D(), pdf,
                                     BxDFType::BSDF_ALL, &flags);
    if (f.isBlack() || pdf == 0.f) break;
    beta *= f * wi.absDot(isect.shading.n) / pdf;
    DAKKU_CHECK(beta.y() > 0, "beta.y() <= 0");
    DAKKU_CHECK(!std::isinf(beta.y()), "beta.y() = inf");
    specularBounce = static_cast<int>(flags & BxDFType::BSDF_SPECULAR) != 0;
    if (static_cast<int>(flags & BxDFType::BSDF_SPECULAR) &&
        static_cast<int>(flags & BxDFType::BSDF_TRANSMISSION)) {
      Float eta = isect.bsdf->eta;
      // update the term that tracks radiance scaling for refraction
      // depending on whether the ray is entering or leaving the
      // medium.
      etaScale *= (wo.dot(isect.n) > 0) ? (eta * eta) : 1 / (eta * eta);
    }
    ray = isect.spawnRay(wi);
    // possibly terminate the path with Russian roulette.
    // factor out radiance scaling due to refraction in rrBeta.
    Spectrum rrBeta = beta * etaScale;
    if (rrBeta.maxComponentValue() < rrThreshold && bounces > 3) {
      Float q =
          std::max(static_cast<Float>(0.05), 1 - rrBeta.maxComponentValue());
      if (sampler.get1D() < q) break;
      beta /= 1 - q;
      DAKKU_CHECK(!std::isinf(beta.y()), "beta.y() = inf");
    }
  }
  return L;
}

void PathIntegrator::unserialize(const Json &json, InputStream *stream) {
  SamplerIntegrator::unserialize(json, stream);
  DAKKU_UNSER_JIE(maxDepth, 10);
  Camera *camera = renderState.camera.get();
  Bounds2i pixelBounds = camera->film->getSampleBounds();
  construct(maxDepth, camera, renderState.sampler.get(), pixelBounds,
            rrThreshold);
}

void PathIntegrator::serialize(Json &json, OutputStream *stream) const {
  SamplerIntegrator::serialize(json, stream);
  DAKKU_SER_J(maxDepth);
}

DAKKU_END