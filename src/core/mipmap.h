#ifndef DAKKU_CORE_MIPMAP_H_
#define DAKKU_CORE_MIPMAP_H_
#include <core/blocked_array.h>
#include <core/vector.h>

#include <utility>

namespace dakku {

/**
 * @brief image wrap mode
 *
 */
enum class ImageWrapMode { REPEAT, BLACK, CLAMP };

/**
 * @brief resample weight of the four nearby (continuous) texels
 *
 */
struct ResampleWeight {
  /// first texel index
  int firstTexel;
  /// resample weight of the four texels
  float weight[4];
};

/**
 * @brief mipmap class
 *
 * @tparam T data type
 */
template <typename T>
requires(std::is_same_v<T, float> || std::is_same_v<T, Spectrum>) class MipMap {
 public:
  explicit MipMap(Point2i resolution, std::span<const T> data,
                  bool doTrilinear = false, float maxAnisotropy = 8.0f,
                  ImageWrapMode wrapMode = ImageWrapMode::REPEAT);

  [[nodiscard]] int width() const { return resolution.x(); }
  [[nodiscard]] int height() const { return resolution.y(); }
  [[nodiscard]] size_t levels() const { return pyramid.size(); }

 private:
  /// do trilinear interpolation
  const bool doTrilinear;
  /// max anisotropy
  const float maxAnisotropy;
  /// image wrap mode
  const ImageWrapMode wrapMode;
  /// resolution
  Point2i resolution;
  /// mipmap pyramid
  std::vector<std::unique_ptr<BlockedArray<T>>> pyramid;
};

template <typename T>
requires(std::is_same_v<T, float> ||
         std::is_same_v<T, Spectrum>) MipMap<T>::MipMap(Point2i resolution,
                                                        std::span<const T> data,
                                                        bool doTrilinear,
                                                        float maxAnisotropy,
                                                        ImageWrapMode wrapMode)
    : resolution(std::move(resolution)),
      doTrilinear(doTrilinear),
      maxAnisotropy(maxAnisotropy),
      wrapMode(wrapMode) {
  // TODO: finish this
}
}  // namespace dakku
#endif