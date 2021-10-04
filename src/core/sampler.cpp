//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/sampler.h>
#include <dakku/core/camera.h>
namespace dakku {

Sampler::Sampler(std::uint32_t samplesPerPixel) : samplesPerPixel(samplesPerPixel) {}

CameraSample Sampler::getCameraSample(const Point2i &pRaster) {
  CameraSample ret;
  Point2f sample = get2D();
  ret.pFilm.x = static_cast<Float>(pRaster.x) + sample.x;
  ret.pFilm.y = static_cast<Float>(pRaster.y) + sample.y;
  return ret;
}
}