#include <core/film.h>
#include <core/fstream.h>

DAKKU_BEGIN

void Film::serialize(Json &json, OutputStream *stream) const {
  DAKKU_SER_J(fullResolution);
  DAKKU_SER_J(diagonal);
  DAKKU_SER_J(fileName);
  DAKKU_SER_J(cropWindow);

  if (size_t size = sizeof(Pixel) * croppedPixelBounds.area();
      stream->writeBytes(pixels.get(), size) != size) {
    DAKKU_ERR("failed to serialize pixels");
  }
}

void Film::unserialize(const Json &json, InputStream *stream) {
  DAKKU_UNSER_J(fullResolution);
  DAKKU_UNSER_JI(diagonal);
  DAKKU_UNSER_JIE(fileName, "dakku.png");
  DAKKU_UNSER_JI(cropWindow);
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
}
DAKKU_END
