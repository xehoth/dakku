#include <core/film.h>
#include <core/fstream.h>
#include <core/class.h>
#include <core/spectrum.h>
#include <core/relative.h>
#include <algorithm>
#include <execution>
#include <OpenImageIO/imageio.h>

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
          static_cast<int>(std::ceil(static_cast<Float>(fullResolution.x()) *
                                     cropWindow.pMin.x())),
          static_cast<int>(std::ceil(static_cast<Float>(fullResolution.y()) *
                                     cropWindow.pMin.y()))),
      Point2i(
          static_cast<int>(std::ceil(static_cast<Float>(fullResolution.x()) *
                                     cropWindow.pMax.x())),
          static_cast<int>(std::ceil(static_cast<Float>(fullResolution.y()) *
                                     cropWindow.pMax.y()))));
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
  // span with filter radius
  return Bounds2i(Bounds2f(floor(Point2f(croppedPixelBounds.pMin) +
                                 Vector2f(0.5f, 0.5f) - filter->radius),
                           ceil(Point2f(croppedPixelBounds.pMax) -
                                Vector2f(0.5f, 0.5f) + filter->radius)));
}

std::unique_ptr<FilmTile> Film::getFilmTile(const Bounds2i &sampleBounds) {
  Vector2f halfPixel(0.5, 0.5);
  auto floatBounds = Bounds2f(sampleBounds);
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
  for (const Point2i &pixel : tile->getPixelBounds()) {
    const FilmTilePixel &tilePixel = tile->getPixel(pixel);
    Pixel &mergePixel = getPixel(pixel);
    Float xyz[3];
    tilePixel.contribSum.toXyz(xyz);
    for (int i = 0; i < 3; ++i) mergePixel.xyz[i] += xyz[i];
    mergePixel.filterWeightSum += tilePixel.filterWeightSum;
  }
}

void Film::writeImage() {
  auto rgb = std::make_unique<Float[]>(3 * croppedPixelBounds.area());
  int offset = 0;
  for (const Point2i &p : croppedPixelBounds) {
    // convert pixel XYZ color to RGB
    Pixel &pixel = getPixel(p);
    xyzToRgb(pixel.xyz, std::span<Float, 3>{&rgb[3 * offset], 3});

    // normalize pixel with weight sum
    Float filterWeightSum = pixel.filterWeightSum;
    if (filterWeightSum != 0) {
      Float invWt = static_cast<Float>(1) / filterWeightSum;
      rgb[3 * offset] =
          std::max(static_cast<Float>(0), rgb[3 * offset] * invWt);
      rgb[3 * offset + 1] =
          std::max(static_cast<Float>(0), rgb[3 * offset + 1] * invWt);
      rgb[3 * offset + 2] =
          std::max(static_cast<Float>(0), rgb[3 * offset + 2] * invWt);
    }

    ++offset;
  }

  // write RGB image
  std::string path = RelativeRoot::instance().get() + fileName;
  std::string extension = std::filesystem::path(path).extension().string();
  if (extension == ".png" || extension == ".jpg") {
    std::transform(
        std::execution::par, rgb.get(), rgb.get() + 3 * croppedPixelBounds.area(),
        rgb.get(),
        [](Float v) { return std::clamp<Float>(gammaCorrect(v), 0, 1); });
  }
  DAKKU_INFO("writing image {} with bounds {}", path, croppedPixelBounds);
  std::unique_ptr<OIIO::ImageOutput> out = OIIO::ImageOutput::create(path);
  if (!out) return;
  OIIO::ImageSpec spec(croppedPixelBounds.diagonal().x(),
                       croppedPixelBounds.diagonal().y(), 3,
                       OIIO::TypeDesc::FLOAT);
  spec.full_x = 0;
  spec.full_y = 0;
  spec.full_width = fullResolution.x();
  spec.full_height = fullResolution.y();
  spec.x = croppedPixelBounds.pMin.x();
  spec.y = croppedPixelBounds.pMin.y();
  out->open(path, spec);
  out->write_image(OIIO::TypeDesc::FLOAT, rgb.get());
  out->close();
}
DAKKU_END
