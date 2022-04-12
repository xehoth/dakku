#ifndef DAKKU_CORE_FILM_H_
#define DAKKU_CORE_FILM_H_
#include <core/serialization.h>
#include <core/bounds.h>
#include <core/spectrum.h>
#include <core/memory.h>
#include <core/filter.h>
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
  explicit Film(const Property &);

  /**
   * @brief get the area to be sampled
   * because the pixel reconstruction filter spans a number of pixels
   * @return the sample bounds
   */
  [[nodiscard]] Bounds2i getSampleBounds() const;

  /**
   * @brief get the film tile according to the given sample bounds
   * @param sampleBounds given region of the image
   * @return the film tile
   */
  std::unique_ptr<FilmTile> getFilmTile(const Bounds2i &sampleBounds);

  /**
   * @brief merge a film tile into the film
   * @param tile the given film tile
   */
  void mergeFilmTile(std::unique_ptr<FilmTile> tile);

  /**
   * @brief write image to `fileName`
   *
   * @param splatScale scale splat values
   */
  void writeImage(float splatScale = 1) const;

  /**
   * @brief write image to `buffer`
   *
   * @param splatScale scale splat values
   */
  void writeImageTo(std::span<float> buffer, float splatScale = 1) const;

  /**
   * @brief write image to `buffer`
   *
   * @param splatScale scale splat values
   */
  void writeImageTo(std::vector<std::uint8_t> &buffer,
                    float splatScale = 1) const;

  void serialize(OutputStream *stream) const override;
  void deserialize(InputStream *stream) override;

  /**
   * @brief Get the Cropped Pixel Bounds object
   *
   */
  [[nodiscard]] Bounds2i getCroppedPixelBounds() const;

  /// full resolution of the film
  Point2i fullResolution;
  /// filter
  std::unique_ptr<Filter> filter{};
  /// file name to save image
  const std::string fileName;
  /// film crop window bounds
  Bounds2i croppedPixelBounds;

 private:
  /**
   * @brief pixel struct
   *
   */
  struct Pixel {
    /// color data (in xyz)
    float xyz[3];
    /// sum of the filter weight
    float filterWeightSum;
    /// splat values to pixels, needed by bidirectional methods
    std::atomic<float> splatXyz[3];
    float _padding;
  };

  /// pixels data
  std::unique_ptr<Pixel[]> pixels{};

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

  Pixel &getPixel(const Point2i &p) {
    return const_cast<Pixel &>(static_cast<const Film &>(*this).getPixel(p));
  }

  [[nodiscard]] const Pixel &getPixel(const Point2i &p) const {
    DAKKU_CHECK(insideExclusive(p, croppedPixelBounds), "index out of range");
    int width = croppedPixelBounds.pMax.x() - croppedPixelBounds.pMin.x();
    int offset = (p.x() - croppedPixelBounds.pMin.x()) +
                 (p.y() - croppedPixelBounds.pMin.y()) * width;
    return pixels[offset];
  }
};

/**
 * @brief film tile
 */
class FilmTile {
 public:
  explicit FilmTile(const Bounds2i &pixelBounds, const Vector2f &filterRadius,
                    std::span<const float> filterTable, int filterTableSize,
                    float maxSampleLuminance = INF)
      : pixelBounds(pixelBounds),
        filterRadius(filterRadius),
        invFilterRadius(1 / filterRadius.x(), 1 / filterRadius.y()),
        filterTable(filterTable),
        filterTableSize(filterTableSize),
        maxSampleLuminance(maxSampleLuminance) {
    pixels.resize(std::max(0, pixelBounds.area()));
  }

  /**
   * @brief Get the Pixel object
   *
   */
  FilmTilePixel &getPixel(const Point2i &p) {
    return const_cast<FilmTilePixel &>(
        static_cast<const FilmTile &>(*this).getPixel(p));
  }

  /**
   * @brief Get the Pixel object
   *
   */
  [[nodiscard]] const FilmTilePixel &getPixel(const Point2i &p) const {
    DAKKU_CHECK(insideExclusive(p, pixelBounds), "index out of range: {}", p);
    int width = pixelBounds.pMax.x() - pixelBounds.pMin.x();
    int offset =
        (p.x() - pixelBounds.pMin.x()) + (p.y() - pixelBounds.pMin.y()) * width;
    return pixels[offset];
  }

  /**
   * @brief Get the Pixel Bounds object
   *
   */
  [[nodiscard]] Bounds2i getPixelBounds() const { return pixelBounds; }

  /**
   * @brief add sample to the film tile
   *
   */
  void addSample(const Point2f &pFilm, Spectrum L, float sampleWeight = 1) {
    if (L.y() > maxSampleLuminance) L *= maxSampleLuminance / L.y();
    // compute sample's raster bounds
    Point2f pFilmDiscrete = pFilm - Vector2f(0.5, 0.5);
    Point2i p0 = Point2i(ceil(pFilmDiscrete - filterRadius));
    Point2i p1 = Point2i(floor(pFilmDiscrete + filterRadius)) + Point2i(1, 1);
    p0 = max(p0, pixelBounds.pMin);
    p1 = min(p1, pixelBounds.pMax);

    // Loop over filter support and add sample to pixel arrays

    // recompute x and y filter table offsets
    int *ifx = DAKKU_ALLOCA(int, p1.x() - p0.x());

    for (int x = p0.x(); x < p1.x(); ++x) {
      float fx =
          std::abs((static_cast<float>(x) - pFilmDiscrete.x()) *
                   invFilterRadius.x() * static_cast<float>(filterTableSize));
      ifx[x - p0.x()] =
          std::min(static_cast<int>(std::floor(fx)), filterTableSize - 1);
    }
    int *ify = DAKKU_ALLOCA(int, p1.y() - p0.y());
    for (int y = p0.y(); y < p1.y(); ++y) {
      float fy =
          std::abs((static_cast<float>(y) - pFilmDiscrete.y()) *
                   invFilterRadius.y() * static_cast<float>(filterTableSize));
      ify[y - p0.y()] =
          std::min(static_cast<int>(std::floor(fy)), filterTableSize - 1);
    }
    for (int y = p0.y(); y < p1.y(); ++y) {
      for (int x = p0.x(); x < p1.x(); ++x) {
        // evaluate filter value at (x, y) pixel
        int offset = ify[y - p0.y()] * filterTableSize + ifx[x - p0.x()];
        float filterWeight = filterTable[offset];

        // update pixel values with filtered sample contribution
        FilmTilePixel &pixel = getPixel(Point2i(x, y));
        pixel.contribSum += L * sampleWeight * filterWeight;
        pixel.filterWeightSum += filterWeight;
      }
    }
  }

 private:
  const Bounds2i pixelBounds;
  const Vector2f filterRadius, invFilterRadius;
  std::span<const float> filterTable;
  const int filterTableSize;
  std::vector<FilmTilePixel> pixels;
  const float maxSampleLuminance;
  friend class Film;
};
}  // namespace dakku
#endif
