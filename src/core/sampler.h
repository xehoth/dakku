#ifndef DAKKU_SRC_CORE_SAMPLER_H_
#define DAKKU_SRC_CORE_SAMPLER_H_
#include <core/sobject.h>
#include <core/vector.h>

DAKKU_BEGIN
class Sampler : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Sampler);

  virtual void startPixel(const Point2i &p);
  virtual Float get1D() = 0;
  virtual Point2f get2D() = 0;
  virtual bool startNextSample();
  virtual std::unique_ptr<Sampler> clone(int seed) = 0;
  std::int64_t samplesPerPixel{};

 protected:
  Point2i currentPixel;
  std::int64_t currentPixelSampleIndex{};
};

DAKKU_END
#endif  // DAKKU_SRC_CORE_SAMPLER_H_
