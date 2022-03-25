#include <samplers/random.h>

DAKKU_BEGIN

void RandomSampler::serialize(Json &json, OutputStream *stream) const {
  DAKKU_ERR("unimplemented");
}

void RandomSampler::unserialize(const Json &json, InputStream *stream) {
  int nSamples;
  DAKKU_UNSER_JIE(nSamples, 4);
  this->samplesPerPixel = nSamples;
  int seed;
  DAKKU_UNSER_JIE(seed, 0);
  rng.setSequence(seed);
}

Float RandomSampler::get1D() {
  DAKKU_CHECK(currentPixelSampleIndex < samplesPerPixel,
              "cur sample index >= spp!");
  return rng.uniformFloat();
}

Point2f RandomSampler::get2D() {
  DAKKU_CHECK(currentPixelSampleIndex < samplesPerPixel,
              "cur sample index >= spp!");
  return Point2f(rng.uniformFloat(), rng.uniformFloat());
}

void RandomSampler::startPixel(const Point2i &p) {
  for (size_t i = 0; i < sampleArray1D.size(); ++i)
    for (size_t j = 0; j < sampleArray1D[i].size(); ++j)
      sampleArray1D[i][j] = rng.uniformFloat();

  for (size_t i = 0; i < sampleArray2D.size(); ++i)
    for (size_t j = 0; j < sampleArray2D[i].size(); ++j)
      sampleArray2D[i][j] = Point2f(rng.uniformFloat(), rng.uniformFloat());
  Sampler::startPixel(p);
}

std::unique_ptr<Sampler> RandomSampler::clone(int seed) {
  auto ret = std::make_unique<RandomSampler>(*this);
  ret->rng.setSequence(seed);
  return ret;
}

DAKKU_END