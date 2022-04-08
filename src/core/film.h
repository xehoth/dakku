#ifndef DAKKU_CORE_FILM_H_
#define DAKKU_CORE_FILM_H_
#include <core/serialization.h>
#include <core/bounds.h>
#include <core/spectrum.h>
#include <mutex>

namespace dakku {

/**
 * @brief film tile pixel
 *
 */
struct FilmTilePixel {
  /// radiance contribution sum
  Spectrum contribSum;
  /// sum of filter weights
  float filterWeightSum;
};

/**
 * @brief film class
 *
 */
class DAKKU_EXPORT_CORE Film : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Film, SerializableObject);

  /**
   * @brief Construct a new Film object
   *
   * @param fullResolution full resolution
   * @param cropWindow crop window, default: $(0, 0) \times (1, 1)$
   * @param filter filter
   * @param fileName file name to save image
   * @param scale scale the pixel value
   * @param maxSampleLuminance max sample luminance
   */
  explicit Film(const Point2i &fullResolution, const Bounds2f &cropWindow,
                std::unique_ptr<Filter> filter, std::string fileName,
                float scale, float maxSampleLuminance = INF);

  void serialize(OutputStream *stream) const override;
  void deserialize(InputStream *stream) override;

 public:
  /**
   * @brief pixel struct
   *
   */
  struct alignas(32) Pixel {
    /// color data (in xyz)
    float xyz[3];
    /// sum of the filter weight
    float filterWeightSum;
    /// splat values to pixels, needed by bidirectional methods
    std::atomic<float> splatXyz[3];
  };

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  /// pixels data
  std::unique_ptr<Pixel[]> pixels{};
  /// full resolution of the film
  Point2i fullResolution;
  /// filter
  std::unique_ptr<Filter> filter{};
  /// file name to save image
  const std::string fileName;
  /// film crop window bounds
  Bounds2i croppedPixelBounds;

  /// filter table width
  static constexpr size_t filterTableWidth = 16;

  /// filter table
  std::array<float, filterTableWidth * filterTableWidth> filterTable{};

  /// mutex to protect tile merge
  std::mutex mutex;

  /// pixel value scale
  const float scale;

  /// max sample luminance
  const float maxSampleLuminance;

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};
}  // namespace dakku
#endif
