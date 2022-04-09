#ifndef DAKKU_IMAGEIO_IMAGEIO_H_
#define DAKKU_IMAGEIO_IMAGEIO_H_
#include <imageio/fwd.h>
#include <filesystem>

namespace dakku {

/**
 * @brief write image to path
 * 
 * @param path file path
 * @param rgb image data
 * @param totalResX total resolution x (width)
 * @param totalResY total resolution y (height)
 * @param outBoundsMinX crop window bounds (pMin.x)
 * @param outBoundsMinY crop window bounds (pMin.y)
 * @param outBoundsMaxX crop window bounds (pMax.x)
 * @param outBoundsMaxY crop window bounds (pMax.y)
 * @return DAKKU_EXPORT_IMAGEIO 
 */
DAKKU_EXPORT_IMAGEIO bool writeImage(const std::filesystem::path &path,
                                     const float *rgb, int totalResX,
                                     int totalResY, int outBoundsMinX = 0,
                                     int outBoundsMinY = 0,
                                     int outBoundsMaxX = -1,
                                     int outBoundsMaxY = -1);
}
#endif