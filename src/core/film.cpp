#include <core/film.h>
#include <core/filter.h>
#include <core/relative.h>
#include <imageio/imageio.h>

namespace dakku {
Film::Film(const Point2i &fullResolution, const Bounds2f &cropWindow,
           std::unique_ptr<Filter> filter, std::string fileName, float scale,
           float maxSampleLuminance)
    : fullResolution(fullResolution),
      filter(std::move(filter)),
      fileName(std::move(fileName)),
      scale(scale),
      maxSampleLuminance(maxSampleLuminance) {
  croppedPixelBounds =
      Bounds2i(Point2i(std::ceil(static_cast<float>(fullResolution.x()) *
                                 cropWindow.pMin.x()),
                       std::ceil(static_cast<float>(fullResolution.y()) *
                                 cropWindow.pMin.y())),
               Point2i(std::ceil(static_cast<float>(fullResolution.x()) *
                                 cropWindow.pMax.x()),
                       std::ceil(static_cast<float>(fullResolution.y()) *
                                 cropWindow.pMax.y())));
  DAKKU_INFO("create film res: {}, crop: {}, crop bounds: {}", fullResolution,
             cropWindow, croppedPixelBounds);
  pixels = std::make_unique<Pixel[]>(croppedPixelBounds.area());

  // precompute filter weight table
  for (int y = 0, offset = 0; y < static_cast<int>(filterTableWidth); ++y) {
    for (int x = 0; x < static_cast<int>(filterTableWidth); ++x, ++offset) {
      Point2f p((x + 0.5) * filter->radius.x() / filterTableWidth,
                (y + 0.5) * filter->radius.y() / filterTableWidth);
      filterTable[offset] = filter->evaluate(p);
    }
  }
}

/**
 * @brief Get the Crop Window object, if not have, return the `value`
 *
 */
Bounds2f getCropWindow(const Property &p,
                       const Bounds2f &value = Bounds2f{{0, 0}, {1, 1}}) {
  if (p.getObject().contains("cropWindow")) {
    const auto &arr = p["cropWindow"].getArray();
    return Bounds2f{arr[0].getVector(), arr[1].getVector()};
  } else {
    return value;
  }
}

/**
 * @brief Create a Filter object
 *
 */
std::unique_ptr<Filter> getFilter(const Property &p) {
  const auto &filter = p["filter"];
  std::string name = filter.getStringIf("class", "BoxFilter");
  return std::unique_ptr<Filter>{
      dynamic_cast<Filter *>(Class::instance().create(name, filter))};
}

Film::Film(const Property &p)
    : Film(p.getVectorIf("fullResolution", Point2i{1280, 720}),
           getCropWindow(p), getFilter(p),
           p.getStringIf("fileName", "dakku.png"), p.getNumberIf("scale", 1.0f),
           p.getNumberIf("maxSampleLuminance", INF)) {}

void Film::serialize(OutputStream *stream) const {
  stream->writeBytes(pixels.get(), sizeof(Pixel) * croppedPixelBounds.area());
}

void Film::deserialize(InputStream *stream) {
  stream->readBytes(pixels.get(), sizeof(Pixel) * croppedPixelBounds.area());
}

Bounds2i Film::getSampleBounds() const {
  // span with filter radius
  return Bounds2i{Bounds2f{floor(Point2f(croppedPixelBounds.pMin) +
                                 Vector2f(0.5f, 0.5f) - filter->radius),
                           ceil(Point2f(croppedPixelBounds.pMax) -
                                Vector2f(0.5f, 0.5f) + filter->radius)}};
}

std::unique_ptr<FilmTile> Film::getFilmTile(const Bounds2i &sampleBounds) {
  Vector2f halfPixel(0.5, 0.5);
  auto floatBounds = Bounds2f(sampleBounds);
  Point2i p0 = Point2i(ceil(floatBounds.pMin - halfPixel - filter->radius));
  Point2i p1 = Point2i(floor(floatBounds.pMax - halfPixel + filter->radius)) +
               Point2i(1, 1);
  Bounds2i tilePixelBounds = Bounds2i(p0, p1) & croppedPixelBounds;
  return std::make_unique<FilmTile>(tilePixelBounds, filter->radius,
                                    filterTable, filterTableWidth,
                                    maxSampleLuminance);
}

void Film::mergeFilmTile(std::unique_ptr<FilmTile> tile) {
  DAKKU_DEBUG("merging film tile: {}", tile->pixelBounds);
  std::lock_guard<std::mutex> lock{mutex};
  for (const Point2i &pixel : tile->getPixelBounds()) {
    const FilmTilePixel &tilePixel = tile->getPixel(pixel);
    Pixel &mergePixel = getPixel(pixel);
    float xyz[3];
    tilePixel.contribSum.toXyz(xyz);
    for (int i = 0; i < 3; ++i) mergePixel.xyz[i] += xyz[i];
    mergePixel.filterWeightSum += tilePixel.filterWeightSum;
  }
}

void Film::writeImage(float splatScale) {
  auto rgb = std::make_unique<float[]>(3 * croppedPixelBounds.area());
  int offset = 0;
  for (const Point2i &p : croppedPixelBounds) {
    // convert pixel XYZ color to RGB
    Pixel &pixel = getPixel(p);
    xyzToRgb(pixel.xyz, std::span<float, 3>{&rgb[3 * offset], 3});

    // normalize pixel with weight sum
    float filterWeightSum = pixel.filterWeightSum;
    if (filterWeightSum != 0) {
      float invWt = 1.0f / filterWeightSum;
      rgb[3 * offset] = std::max(0.0f, rgb[3 * offset] * invWt);
      rgb[3 * offset + 1] = std::max(0.0f, rgb[3 * offset + 1] * invWt);
      rgb[3 * offset + 2] = std::max(0.0f, rgb[3 * offset + 2] * invWt);
    }

    // add splat value at pixel
    float splatRgb[3];
    float splatXyz[3] = {pixel.splatXyz[0], pixel.splatXyz[1],
                         pixel.splatXyz[2]};
    xyzToRgb(splatXyz, splatRgb);
    rgb[3 * offset] += splatScale * splatRgb[0];
    rgb[3 * offset + 1] += splatScale * splatRgb[1];
    rgb[3 * offset + 2] += splatScale * splatRgb[2];

    // scale pixel value by scale
    rgb[3 * offset] *= scale;
    rgb[3 * offset + 1] *= scale;
    rgb[3 * offset + 2] *= scale;
    ++offset;
  }
  std::string path = RelativeRoot::instance().get() + fileName;
  dakku::writeImage(path, rgb.get(), fullResolution.x(), fullResolution.y(),
                    croppedPixelBounds.pMin.x(), croppedPixelBounds.pMin.y(),
                    croppedPixelBounds.pMax.x(), croppedPixelBounds.pMax.y());
}
}  // namespace dakku