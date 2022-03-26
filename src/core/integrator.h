#ifndef DAKKU_SRC_CORE_INTEGRATOR_H_
#define DAKKU_SRC_CORE_INTEGRATOR_H_
#include <core/sobject.h>
#include <core/bounds.h>

DAKKU_BEGIN
class Integrator : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Integrator);
  virtual void render(const Scene &scene) = 0;
};

Spectrum uniformSampleOneLight(const Interaction &it, const Scene &scene,
                               MemoryArena &arena, Sampler &sampler);
Spectrum estimateDirect(const Interaction &it, const Point2f &uScattering,
                        const Light &light, const Point2f &uLight,
                        const Scene &scene, Sampler &sampler,
                        MemoryArena &arena, bool specular = false);

class SamplerIntegrator : public Integrator {
 public:
  DAKKU_DECLARE_OBJECT(SamplerIntegrator);

  void construct(const Camera *_camera, Sampler *_sampler,
                 const Bounds2i &_pixelBounds) {
    this->camera = _camera;
    this->sampler = _sampler;
    this->pixelBounds = _pixelBounds;
  }

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;
  virtual void preprocess(const Scene &scene, Sampler &sampler) {}
  void render(const Scene &scene) override;
  virtual Spectrum radiance(const Ray &ray, const Scene &scene,
                            Sampler &sampler, MemoryArena &arena,
                            int depth = 0) const = 0;

 protected:
  const Camera *camera{};

 private:
  Sampler *sampler{};
  Bounds2i pixelBounds;
  int totalTiles{};
  std::vector<char> finishedTiles;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_INTEGRATOR_H_
