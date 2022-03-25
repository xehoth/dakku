#ifndef DAKKU_SRC_SAMPLERS_RANDOM_H_
#define DAKKU_SRC_SAMPLERS_RANDOM_H_
#include <core/sampler.h>

DAKKU_BEGIN
class RandomSampler : public Sampler {
 public:
  DAKKU_DECLARE_OBJECT(RandomSampler);

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;
  Float get1D() override;
  void startPixel(const Point2i &p) override;
  Point2f get2D() override;
  std::unique_ptr<Sampler> clone(int seed) override;
 private:
  RNG rng;
};
DAKKU_END
#endif  // DAKKU_SRC_SAMPLERS_RANDOM_H_
