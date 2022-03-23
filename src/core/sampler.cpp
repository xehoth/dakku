#include <core/sampler.h>

DAKKU_BEGIN
void Sampler::startPixel(const Point2i &p) {
  currentPixel = p;
  currentPixelSampleIndex = 0;
}

bool Sampler::startNextSample() {
  return ++currentPixelSampleIndex < samplesPerPixel;
}
DAKKU_END
