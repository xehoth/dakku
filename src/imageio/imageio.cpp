#include <imageio/imageio.h>
#include <OpenImageIO/imageio.h>
#include <memory>
#include <cstring>
#include <execution>

namespace dakku {
bool writeImage(const std::filesystem::path &path, const float *rgb,
                int totalResX, int totalResY, int outBoundsMinX,
                int outBoundsMinY, int outBoundsMaxX, int outBoundsMaxY) {
  if (outBoundsMaxX == -1) outBoundsMaxX = totalResX;
  if (outBoundsMaxY == -1) outBoundsMaxY = totalResY;
  std::string extension = path.extension().string();
  size_t size =
      (outBoundsMaxX - outBoundsMinX) * (outBoundsMaxY - outBoundsMinY);
  const float *outRgb = rgb;
  std::unique_ptr<float[]> buffer{};
  if (extension == ".png" || extension == ".jpg") {
    buffer = std::make_unique<float[]>(3 * size);
    std::transform(
        std::execution::par_unseq, rgb, rgb + 3 * size, buffer.get(),
        [](float v) { return std::clamp(gammaCorrect(v), 0.0f, 1.0f); });
    rgb = buffer.get();
  }
  std::unique_ptr<OIIO::ImageOutput> out =
      OIIO::ImageOutput::create(path.string());
  if (!out) return false;
  OIIO::ImageSpec spec(outBoundsMaxX - outBoundsMinX,
                       outBoundsMaxY - outBoundsMinY, 3, OIIO::TypeDesc::FLOAT);
  spec.full_x = 0;
  spec.full_y = 0;
  spec.full_width = totalResX;
  spec.full_height = totalResY;
  spec.x = outBoundsMinX;
  spec.y = outBoundsMinY;
  out->open(path.string(), spec);
  out->write_image(OIIO::TypeDesc::FLOAT, outRgb);
  out->close();
  return true;
}

float *readImage(const std::filesystem::path &path, int &resX, int &resY,
                 bool flipVertical, int gamma, float scale) {
  auto in = OIIO::ImageInput::open(path.string());
  if (!in) return nullptr;
  const OIIO::ImageSpec &spec = in->spec();
  resX = spec.width;
  resY = spec.height;
  std::vector<float> rawImg(resX * resY * spec.nchannels);
  if (flipVertical) {
    const int scanline_size = resY * spec.nchannels;
    // flip y
    in->read_image(OIIO::TypeFloat, rawImg.data() + (resY - 1) * scanline_size,
                   OIIO::AutoStride,
                   -scanline_size * static_cast<int>(sizeof(float)),
                   OIIO::AutoStride);
    in->close();
  } else {
    in->read_image(OIIO::TypeFloat, rawImg.data());
  }
  auto *rgb = new float[resX * resY * 3];
  for (int i = 0; i < resX * resY; ++i) {
    int j = 0;
    for (; j < spec.nchannels && j < 3; ++j) {
      rgb[i * 3 + j] = rawImg[i * spec.nchannels + j];
    }
    for (; j < 3; ++j) rgb[i * 3 + j] = 0;
  }
  std::string extension = path.extension().string();
  if (gamma == 1 ||
      (gamma == -1 && (extension == ".png" || extension == ".jpg"))) {
    std::transform(std::execution::par_unseq, rgb, rgb + 3 * resX * resY, rgb,
                   [scale](float v) { return inverseGammaCorrect(v) * scale; });
  }
  return rgb;
}
}  // namespace dakku