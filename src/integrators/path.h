#ifndef DAKKU_SRC_INTEGRATORS_PATH_H_
#define DAKKU_SRC_INTEGRATORS_PATH_H_
#include <core/integrator.h>

DAKKU_BEGIN
class PathIntegrator : public SamplerIntegrator {
 public:
  DAKKU_DECLARE_OBJECT(PathIntegrator);

  void serialize(Json &json, OutputStream *) const override;
  void unserialize(const Json &json, InputStream *) override;

  void construct(int _maxDepth, const Camera *camera, Sampler *sampler,
                 const Bounds2i &pixelBounds, Float _rrThreshold = 1);

  void preprocess(const Scene &scene, Sampler &sampler) override;
  Spectrum radiance(const Ray &ray, const Scene &scene, Sampler &sampler,
                    MemoryArena &arena, int depth = 0) const override;

 private:
  int maxDepth{};
  Float rrThreshold{1};
};
DAKKU_END
#endif  // DAKKU_SRC_INTEGRATORS_PATH_H_
