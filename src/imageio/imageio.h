#ifndef DAKKU_IMAGEIO_IMAGEIO_H_
#define DAKKU_IMAGEIO_IMAGEIO_H_
#include <imageio/fwd.h>
#include <filesystem>
#include <memory>

namespace dakku {

/**
 * @brief write image to path
 *
 * @param path file path
 * @param totalResX total resolution x (width)
 * @param totalResY total resolution y (height)
 * @param outBoundsMinX crop window bounds (pMin.x)
 * @param outBoundsMinY crop window bounds (pMin.y)
 * @param outBoundsMaxX crop window bounds (pMax.x)
 * @param outBoundsMaxY crop window bounds (pMax.y)
 */
DAKKU_EXPORT_IMAGEIO bool writeImage(const std::filesystem::path &path,
                                     const float *rgb, int totalResX,
                                     int totalResY, int outBoundsMinX = 0,
                                     int outBoundsMinY = 0,
                                     int outBoundsMaxX = -1,
                                     int outBoundsMaxY = -1);

/**
 * @brief read image from file
 * **need to free memory manually**
 *
 * @param path image file path
 * @param rgb image buffer
 * @param [out] resX image resolution x (width)
 * @param [out] resY image resolution y (height)
 * @param flipVertical flip y
 * @param gamma do inverse gamma correction: -1 auto, 0 not, 1 do
 * @param scale scale the image value
 */
DAKKU_EXPORT_IMAGEIO float *readImage(const std::filesystem::path &path,
                                      int &resX, int &resY,
                                      bool flipVertical = true, int gamma = -1,
                                      float scale = 1);
}  // namespace dakku
#endif