#include <core/integrator.h>
#include <core/camera.h>
#include <core/sampler.h>
#include <core/scene.h>
#include <core/film.h>
#include <core/reflection.h>
#include <core/light.h>
#include <core/sampling.h>
#include <core/primitive.h>
#include <execution>
#include <atomic>

DAKKU_BEGIN

Spectrum uniformSampleOneLight(const Interaction &it, const Scene &scene,
                               MemoryArena &arena, Sampler &sampler) {
  // randomly choose a single light to sample, light
  int nLights = static_cast<int>(scene.lights.size());
  if (nLights == 0) return Spectrum{0};
  int lightNum =
      std::min(static_cast<int>(sampler.get1D() * static_cast<Float>(nLights)),
               nLights - 1);
  Float lightPdf = static_cast<Float>(1) / static_cast<Float>(nLights);
  const Light *light = scene.lights[lightNum];
  Point2f uLight = sampler.get2D();
  Point2f uScattering = sampler.get2D();
  return estimateDirect(it, uScattering, *light, uLight, scene, sampler,
                        arena) /
         lightPdf;
}

Spectrum estimateDirect(const Interaction &it, const Point2f &uScattering,
                        const Light &light, const Point2f &uLight,
                        const Scene &scene, Sampler &sampler,
                        MemoryArena &arena, bool specular) {
  BxDFType bsdfFlags =
      specular ? BxDFType::BSDF_ALL
               : BxDFType::BSDF_ALL & static_cast<BxDFType>(~static_cast<int>(
                                          BxDFType::BSDF_SPECULAR));
  Spectrum Ld(0);
  // sample light source with multiple importance sampling
  Vector3f wi;
  Float lightPdf = 0, scatteringPdf = 0;
  VisibilityTester visibility;
  Spectrum Li = light.sampleLi(it, uLight, wi, lightPdf, visibility);
  if (lightPdf > 0 && !Li.isBlack()) {
    Spectrum f;
    const auto &isect = reinterpret_cast<const SurfaceInteraction &>(it);
    f = isect.bsdf->f(isect.wo, wi, bsdfFlags) * wi.absDot(isect.shading.n);
    scatteringPdf = isect.bsdf->pdf(isect.wo, wi, bsdfFlags);
    if (!f.isBlack()) {
      if (!visibility.unoccluded(scene)) {
        Li = Spectrum(0);
      }

      // add light's contribution to reflected radiance
      if (!Li.isBlack()) {
        if (isDeltaLight(light.flags))
          Ld += f * Li / lightPdf;
        else {
          Float weight = powerHeuristic(1, lightPdf, 1, scatteringPdf);
          Ld += f * Li * weight / lightPdf;
        }
      }
    }
  }
  // sample BSDF with multiple importance sampling
  if (!isDeltaLight(light.flags)) {
    Spectrum f;
    bool sampledSpecular = false;
    // sample scattered direction for surface interactions
    BxDFType sampledType;
    const auto &isect = reinterpret_cast<const SurfaceInteraction &>(it);
    f = isect.bsdf->sampleF(isect.wo, wi, uScattering, scatteringPdf, bsdfFlags,
                            &sampledType);
    f *= wi.absDot(isect.shading.n);
    sampledSpecular =
        static_cast<int>(sampledType & BxDFType::BSDF_SPECULAR) != 0;
    if (!f.isBlack() && scatteringPdf > 0) {
      // account for light contributions along sampled direction wi
      Float weight = 1;

      // find intersection and compute transmittance
      SurfaceInteraction lightIsect;
      Ray ray = it.spawnRay(wi);
      Spectrum Tr(1);
      bool foundSurfaceInteraction = scene.intersect(ray, lightIsect);

      if (!sampledSpecular) {
        lightPdf = light.pdfLi(it, lightIsect, foundSurfaceInteraction, wi);
        if (lightPdf == 0) return Ld;
        weight = powerHeuristic(1, scatteringPdf, 1, lightPdf);
      }

      // Add light contribution from material sampling
      Spectrum Li(0);
      if (foundSurfaceInteraction) {
        if (lightIsect.primitive->getAreaLight() == &light)
          Li = lightIsect.emit(-wi);
      } else {
        // TODO: infinite light
      }
      if (!Li.isBlack()) Ld += f * Li * Tr * weight / scatteringPdf;
    }
  }
  return Ld;
}

void SamplerIntegrator::render(const Scene &scene) {
  preprocess(scene, *sampler);

  Bounds2i sampleBounds = camera->film->getSampleBounds();
  Vector2i sampleExtent = sampleBounds.diagonal();
  const int tileSize = 16;
  Point2i nTiles((sampleExtent.x() + tileSize - 1) / tileSize,
                 (sampleExtent.y() + tileSize - 1) / tileSize);
  std::vector<Point2i> tileBuffer;
  tileBuffer.reserve(nTiles.x() * nTiles.y());
  for (int i = 0; i < nTiles.x(); ++i)
    for (int j = 0; j < nTiles.y(); ++j) tileBuffer.emplace_back(i, j);
  std::atomic_int finishedTiles = 0;
  std::for_each(
      std::execution::par, tileBuffer.begin(), tileBuffer.end(),
      [&](const Point2i &tile) {
        // memory arena for tile
        MemoryArena arena;
        // get sampler instance for tile
        int seed = tile.y() * nTiles.x() + tile.x();
        std::unique_ptr<Sampler> tileSampler = sampler->clone(seed);
        // compute sample bounds for tile
        int x0 = sampleBounds.pMin.x() + tile.x() * tileSize;
        int x1 = std::min(x0 + tileSize, sampleBounds.pMax.x());
        int y0 = sampleBounds.pMin.y() + tile.y() * tileSize;
        int y1 = std::min(y0 + tileSize, sampleBounds.pMax.y());
        Bounds2i tileBounds(Point2i(x0, y0), Point2i(x1, y1));
        DAKKU_DEBUG("starting image tile {}", tileBounds);
        // get film tile
        std::unique_ptr<FilmTile> filmTile =
            camera->film->getFilmTile(tileBounds);
        // loop over pixels in the tile
        for (const Point2i &pixel : tileBounds) {
          tileSampler->startPixel(pixel);
          if (!insideExclusive(pixel, pixelBounds)) continue;
          do {
            // get camera sample for current pixel
            CameraSample cameraSample = tileSampler->getCameraSample(pixel);
            // generate camera ray
            Ray ray;
            Float rayWeight = camera->generateRay(cameraSample, ray);
            // evaluate radiance along camera ray
            Spectrum L(0);
            if (rayWeight > 0) L = radiance(ray, scene, *tileSampler, arena);
            // issue warning if unexpected radiance value returned
            if (L.hasNaN()) {
              DAKKU_ERR("not a number radiance for pixel: {}, sample: {}",
                        pixel, tileSampler->currentSampleNumber());
              L = Spectrum(0.f);
            } else if (L.y() < -1e-5) {
              DAKKU_ERR(
                  "negative luminance value, {}, returned for pixel: {}, "
                  "sample: {}",
                  L.y(), pixel, tileSampler->currentSampleNumber());
              L = Spectrum(0.f);
            } else if (std::isinf(L.y())) {
              DAKKU_ERR(
                  "infinite luminance value returned for pixel: {}, sample: {}",
                  pixel, tileSampler->currentSampleNumber());
              L = Spectrum(0.f);
            }
            //            DAKKU_DEBUG("camera sample: {} -> ray: {} -> L = {}",
            //                        cameraSample.pFilm, ray, L);
            // add camera ray's contribution to image
            filmTile->addSample(cameraSample.pFilm, L, rayWeight);
            // release arena memory
            arena.release();
          } while (tileSampler->startNextSample());
        }
        DAKKU_DEBUG("finish image tile: {}", tileBounds);
        // merge image tile into film
        camera->film->mergeFilmTile(std::move(filmTile));
        ++finishedTiles;
        printf(
            "\r%5.2f%%",
            finishedTiles / static_cast<double>(nTiles.x() * nTiles.y()) * 100);
      });
  DAKKU_INFO("render finished");
}

DAKKU_END