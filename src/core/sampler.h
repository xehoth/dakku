#ifndef DAKKU_SRC_CORE_SAMPLER_H_
#define DAKKU_SRC_CORE_SAMPLER_H_
#include <core/sobject.h>
#include <core/vector.h>
#include <core/rng.h>
#include <vector>

DAKKU_BEGIN
class Sampler : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Sampler);

  virtual void startPixel(const Point2i &p);
  virtual Float get1D() = 0;
  virtual Point2f get2D() = 0;
  void requestArray1D(int n);
  void requestArray2D(int n);
  virtual int roundCount(int n) const { return n; }
  const Float *getArray1D(int n);
  const Point2f *getArray2D(int n);
  virtual bool startNextSample();
  virtual std::unique_ptr<Sampler> clone(int seed) = 0;
  virtual bool setSampleNumber(std::int64_t sampleNum);
  [[nodiscard]] std::int64_t currentSampleNumber() const {
    return currentPixelSampleIndex;
  }

  std::int64_t samplesPerPixel{};

 protected:
  Point2i currentPixel;
  std::int64_t currentPixelSampleIndex{};
  std::vector<int> samples1DArraySizes, samples2DArraySizes;
  std::vector<std::vector<Float>> sampleArray1D;
  std::vector<std::vector<Point2f>> sampleArray2D;

 private:
  size_t array1DOffset{}, array2DOffset{};
};

class PixelSampler : public Sampler {
 public:
  DAKKU_DECLARE_OBJECT(PixelSampler);

 protected:
  std::vector<std::vector<Float>> samples1D;
  std::vector<std::vector<Point2f>> samples2D;
  int current1DDimension{};
  int current2DDimension{};
  RNG rng;
};

DAKKU_END
#endif  // DAKKU_SRC_CORE_SAMPLER_H_
