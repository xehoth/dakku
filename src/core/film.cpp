#include <core/film.h>
#include <core/fstream.h>
#include <core/class.h>

DAKKU_BEGIN

void Film::serialize(Json &json, OutputStream *stream) const {
  DAKKU_SER_J(fullResolution);
  DAKKU_SER_J(diagonal);
  DAKKU_SER_J(fileName);
  DAKKU_SER_J(cropWindow);

  filter->serialize(json["filter"], stream);

  if (size_t size = sizeof(Pixel) * croppedPixelBounds.area();
      stream->writeBytes(pixels.get(), size) != size) {
    DAKKU_ERR("failed to serialize pixels");
  }

  DAKKU_SER_J(maxSampleLuminance);
}

void Film::unserialize(const Json &json, InputStream *stream) {
  DAKKU_UNSER_J(fullResolution);
  DAKKU_UNSER_JI(diagonal);
  DAKKU_UNSER_JIE(fileName, "dakku.png");
  DAKKU_UNSER_JI(cropWindow);

  std::string filterType = "BoxFilter";
  Json jFilterDummy;
  const Json *jFilter = &jFilterDummy;
  if (!json.contains("filter")) {
    DAKKU_WARN("film does not have a filter, use default: BoxFilter");
  } else {
    jFilter = &json["filter"];
    if (!jFilter->contains("class")) {
      DAKKU_WARN("filter type is unknown, use default: BoxFilter");
    } else {
      jFilter->at("class").get_to(filterType);
    }
  }
  filter.reset(dynamic_cast<Filter *>(Class::instance().create(filterType)));
  DAKKU_INFO("create filter: {}", filter->getClassName());
  filter->unserialize(*jFilter, stream);

  croppedPixelBounds = Bounds2i(
      Point2i(
          static_cast<int>(std::ceil(fullResolution.x() * cropWindow.pMin.x())),
          static_cast<int>(
              std::ceil(fullResolution.y() * cropWindow.pMin.y()))),
      Point2i(
          static_cast<int>(std::ceil(fullResolution.x() * cropWindow.pMax.x())),
          static_cast<int>(
              std::ceil(fullResolution.y() * cropWindow.pMax.y()))));
  DAKKU_INFO("create film with full resolution {}", fullResolution);
  DAKKU_INFO("crop window of {})", cropWindow);
  DAKKU_INFO("-> croppedPixelBounds {}", croppedPixelBounds);
  pixels = std::make_unique<Pixel[]>(croppedPixelBounds.area());

  if (size_t size = sizeof(Pixel) * croppedPixelBounds.area();
      stream && stream->readBytes(pixels.get(), size) != size) {
    DAKKU_WARN("read pixels size unmatched");
  }

  // precompute filter weight table
  for (int y = 0, offset = 0; y < filterTableWidth; ++y) {
    for (int x = 0; x < filterTableWidth; ++x, ++offset) {
      Point2f p((x + 0.5) * filter->radius.x() / filterTableWidth,
                (y + 0.5) * filter->radius.y() / filterTableWidth);
      filterTable[offset] = filter->evaluate(p);
    }
  }

  DAKKU_UNSER_JI(maxSampleLuminance);
}

Bounds2i Film::getSampleBounds() const {
  return Bounds2i(Bounds2f(floor(Point2f(croppedPixelBounds.pMin) +
                                 Vector2f(0.5f, 0.5f) - filter->radius),
                           ceil(Point2f(croppedPixelBounds.pMax) -
                                Vector2f(0.5f, 0.5f) + filter->radius)));
}

std::unique_ptr<FilmTile> Film::getFilmTile(const Bounds2i &sampleBounds) {
  Vector2f halfPixel(0.5, 0.5);
  Bounds2f floatBounds = Bounds2f(sampleBounds);
  Point2i p0 = Point2i(ceil(floatBounds.pMin - halfPixel - filter->radius));
  Point2i p1 = Point2i(floor(floatBounds.pMax - halfPixel + filter->radius)) +
               Point2i(1, 1);
  Bounds2i tilePixelBounds = intersect(Bounds2i(p0, p1), croppedPixelBounds);
  return std::make_unique<FilmTile>(tilePixelBounds, filter->radius,
                                    filterTable, filterTableWidth,
                                    maxSampleLuminance);
}
void Film::mergeFilmTile(std::unique_ptr<FilmTile> tile) {
  DAKKU_DEBUG("merging film tile: {}", tile->pixelBounds);
  std::lock_guard<std::mutex> lock{mutex};
  for (Point2i pixel : tile->getPixelBounds()) {
    const FilmTilePixel &tilePixel = tile->getPixel(pixel);
    Pixel &mergePixel = getPixel(pixel);
    Float xyz[3];
    tilePixel.contribSum.toXyz(xyz);
    for (int i = 0; i < 3; ++i) mergePixel.xyz[i] += xyz[i];
    mergePixel.filterWeightSum += tilePixel.filterWeightSum;
  }
}
DAKKU_END
