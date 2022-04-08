#include <core/film.h>
#include <core/filter.h>

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
}  // namespace dakku