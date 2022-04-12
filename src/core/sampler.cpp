#include <core/sampler.h>
#include <core/camera.h>

namespace dakku {

CameraSample Sampler::getCameraSample(const Point2i &pRaster) {
  CameraSample cs;
  cs.pFilm = Point2f(pRaster) + get2D();
  return cs;
}

void Sampler::startPixel(const Point2i &p) {
  currentPixel = p;
  currentPixelSampleIndex = 0;
  // reset array offsets for next pixel sample
  array1DOffset = array2DOffset = 0;
}

bool Sampler::startNextSample() {
  // reset array offsets for next pixel sample
  array1DOffset = array2DOffset = 0;
  return ++currentPixelSampleIndex < samplesPerPixel;
}

bool Sampler::setSampleNumber(std::int64_t sampleNum) {
  // reset array offsets for next pixel sample
  array1DOffset = array2DOffset = 0;
  currentPixelSampleIndex = sampleNum;
  return currentPixelSampleIndex < samplesPerPixel;
}

void Sampler::requestArray1D(int n) {
  DAKKU_CHECK(roundCount(n) == n, "invalid n: {}, expected: {}", n,
              roundCount(n));
  samples1DArraySizes.push_back(n);
  sampleArray1D.emplace_back(n * samplesPerPixel);
}

void Sampler::requestArray2D(int n) {
  DAKKU_CHECK(roundCount(n) == n, "invalid n: {}, expected: {}", n,
              roundCount(n));
  samples2DArraySizes.push_back(n);
  sampleArray2D.emplace_back(n * samplesPerPixel);
}

const float *Sampler::getArray1D(int n) {
  if (array1DOffset == sampleArray1D.size()) return nullptr;
  DAKKU_CHECK(samples1DArraySizes[array1DOffset] == n,
              "invalid n: {}, expected: {}", n,
              samples1DArraySizes[array1DOffset]);
  DAKKU_CHECK(currentPixelSampleIndex < samplesPerPixel,
              "cur sample index >= spp!");
  return &sampleArray1D[array1DOffset++][currentPixelSampleIndex * n];
}

const Point2f *Sampler::getArray2D(int n) {
  if (array2DOffset == sampleArray2D.size()) return nullptr;
  DAKKU_CHECK(samples2DArraySizes[array2DOffset] == n,
              "invalid n: {}, expected: {}", n,
              samples2DArraySizes[array2DOffset]);
  DAKKU_CHECK(currentPixelSampleIndex < samplesPerPixel,
              "cur sample index >= spp!");
  return &sampleArray2D[array2DOffset++][currentPixelSampleIndex * n];
}

bool PixelSampler::startNextSample() {
  current1DDimension = current2DDimension = 0;
  return Sampler::startNextSample();
}

bool PixelSampler::setSampleNumber(std::int64_t sampleNum) {
  current1DDimension = current2DDimension = 0;
  return Sampler::setSampleNumber(sampleNum);
}

float PixelSampler::get1D() {
  DAKKU_CHECK(currentPixelSampleIndex < samplesPerPixel,
              "cur sample index >= spp!");
  if (current1DDimension < static_cast<int>(samples1D.size()))
    return samples1D[current1DDimension++][currentPixelSampleIndex];
  else
    return rng.uniformFloat();
}

Point2f PixelSampler::get2D() {
  DAKKU_CHECK(currentPixelSampleIndex < samplesPerPixel,
              "cur sample index >= spp!");
  if (current2DDimension < static_cast<int>(samples2D.size()))
    return samples2D[current2DDimension++][currentPixelSampleIndex];
  else
    return Point2f{rng.uniformFloat(), rng.uniformFloat()};
}
}  // namespace dakku