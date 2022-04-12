#ifndef DAKKU_CORE_SAMPLER_H_
#define DAKKU_CORE_SAMPLER_H_
#include <core/serialization.h>
#include <core/rng.h>

namespace dakku {

class DAKKU_EXPORT_CORE Sampler : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Sampler, SerializableObject);

  virtual void startPixel(const Point2i &p);
  virtual float get1D() = 0;
  virtual Point2f get2D() = 0;
  CameraSample getCameraSample(const Point2i &pRaster);
  void requestArray1D(int n);
  void requestArray2D(int n);
  [[nodiscard]] virtual int roundCount(int n) const { return n; }
  const float *getArray1D(int n);
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
  std::vector<std::vector<float>> sampleArray1D;
  std::vector<std::vector<Point2f>> sampleArray2D;

 private:
  size_t array1DOffset{}, array2DOffset{};
};

class DAKKU_EXPORT_CORE PixelSampler : public Sampler {
 public:
  DAKKU_DECLARE_OBJECT(PixelSampler, Sampler);

  bool startNextSample() override;
  bool setSampleNumber(std::int64_t sampleNum) override;
  float get1D() override;
  Point2f get2D() override;

 protected:
  std::vector<std::vector<float>> samples1D;
  std::vector<std::vector<Point2f>> samples2D;
  int current1DDimension{};
  int current2DDimension{};
  RNG rng;
};
}
#endif