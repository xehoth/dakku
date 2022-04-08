#include <core/film.h>

namespace dakku {
Film::Film(const Property &p) {}

void Film::serialize(OutputStream *stream) const {
  stream->writeBytes(pixels.get(), sizeof(Pixel) * croppedPixelBounds.area());
}

void Film::deserialize(InputStream *stream) {
  stream->readBytes(pixels.get(), sizeof(Pixel) * croppedPixelBounds.area());
}
}  // namespace dakku