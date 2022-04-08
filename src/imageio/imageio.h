#ifndef DAKKU_IMAGEIO_IMAGEIO_H_
#define DAKKU_IMAGEIO_IMAGEIO_H_
#include <imageio/fwd.h>
#include <filesystem>

namespace dakku {

DAKKU_EXPORT_IMAGEIO bool writeImage(const std::filesystem::path &path,
                                     const float *rgb, int totalResX,
                                     int totalResY, int outBoundsMinX = 0,
                                     int outBoundsMinY = 0,
                                     int outBoundsMaxX = -1,
                                     int outBoundsMaxY = -1);
}
#endif