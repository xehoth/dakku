#ifndef DAKKU_SRC_CORE_FILM_H_
#define DAKKU_SRC_CORE_FILM_H_
#include <core/sobject.h>
#include <core/bounds.h>
#include <core/filter.h>
#include <core/spectrum.h>
#include <core/memory.h>
#include <span>
#include <mutex>

DAKKU_BEGIN
/**
 * dakku film tile pixel
 */
struct FilmTilePixel {
  Spectrum contribSum;
  Float filterWeightSum{};
};

/**
 * dakku Film
 */
class Film : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Film, SerializableObject);

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;
  /**
   * get the area to be sampled
   * because the pixel reconstruction filter spans a number of pixels
   * @return the sample bounds
   */
  [[nodiscard]] Bounds2i getSampleBounds() const;
  /**
   * get the film tile according to the given sample bounds
   * @param sampleBounds given region of the image
   * @return the film tile
   */
  std::unique_ptr<FilmTile> getFilmTile(const Bounds2i &sampleBounds);
  /**
   * merge a film tile into the film
   * @param tile the given film tile
   */
  void mergeFilmTile(std::unique_ptr<FilmTile> tile);
  /**
   * write image to [fileName]
   */
  void writeImage();
  Point2i fullResolution;
  Float diagonal{0.35f};
  std::unique_ptr<Filter> filter{};
  std::string fileName;
  Bounds2f cropWindow{Point2f(0, 0), Point2f(1, 1)};
  Bounds2i croppedPixelBounds;

 private:
  struct Pixel {
    Float xyz[3];
    Float filterWeightSum;
  };
  std::unique_ptr<Pixel[]> pixels{};
  static constexpr int filterTableWidth = 16;
  Float filterTable[filterTableWidth * filterTableWidth]{};
  std::mutex mutex;
  Float maxSampleLuminance{INF};

  Pixel &getPixel(const Point2i &p) {
    DAKKU_CHECK(insideExclusive(p, croppedPixelBounds), "index out of range");
    int width = croppedPixelBounds.pMax.x() - croppedPixelBounds.pMin.x();
    int offset = (p.x() - croppedPixelBounds.pMin.x()) +
                 (p.y() - croppedPixelBounds.pMin.y()) * width;
    return pixels[offset];
  }
};

/**
 * dakku film tile
 */
class FilmTile {
 public:
  explicit FilmTile(const Bounds2i &pixelBounds, const Vector2f &filterRadius,
                    std::span<const Float> filterTable, int filterTableSize,
                    Float maxSampleLuminance = INF)
      : pixelBounds(pixelBounds),
        filterRadius(filterRadius),
        invFilterRadius(1 / filterRadius.x(), 1 / filterRadius.y()),
        filterTable(filterTable),
        filterTableSize(filterTableSize),
        maxSampleLuminance(maxSampleLuminance) {
    pixels.resize(std::max(0, pixelBounds.area()));
  }

  FilmTilePixel &getPixel(const Point2i &p) {
    return const_cast<FilmTilePixel &>(
        static_cast<const FilmTile &>(*this).getPixel(p));
  }
  [[nodiscard]] const FilmTilePixel &getPixel(const Point2i &p) const {
    DAKKU_CHECK(insideExclusive(p, pixelBounds), "index out of range: {}", p);
    int width = pixelBounds.pMax.x() - pixelBounds.pMin.x();
    int offset =
        (p.x() - pixelBounds.pMin.x()) + (p.y() - pixelBounds.pMin.y()) * width;
    return pixels[offset];
  }

  [[nodiscard]] Bounds2i getPixelBounds() const { return pixelBounds; }

  void addSample(const Point2f &pFilm, Spectrum L, Float sampleWeight = 1) {
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
      Float fx =
          std::abs((static_cast<Float>(x) - pFilmDiscrete.x()) *
                   invFilterRadius.x() * static_cast<Float>(filterTableSize));
      ifx[x - p0.x()] =
          std::min(static_cast<int>(std::floor(fx)), filterTableSize - 1);
    }
    int *ify = DAKKU_ALLOCA(int, p1.y() - p0.y());
    for (int y = p0.y(); y < p1.y(); ++y) {
      Float fy =
          std::abs((static_cast<Float>(y) - pFilmDiscrete.y()) *
                   invFilterRadius.y() * static_cast<Float>(filterTableSize));
      ify[y - p0.y()] =
          std::min(static_cast<int>(std::floor(fy)), filterTableSize - 1);
    }
    for (int y = p0.y(); y < p1.y(); ++y) {
      for (int x = p0.x(); x < p1.x(); ++x) {
        // evaluate filter value at (x, y) pixel
        int offset = ify[y - p0.y()] * filterTableSize + ifx[x - p0.x()];
        Float filterWeight = filterTable[offset];

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
  std::span<const Float> filterTable;
  const int filterTableSize;
  const Float maxSampleLuminance;
  std::vector<FilmTilePixel> pixels;
  friend class Film;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_FILM_H_
