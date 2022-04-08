#include <imageio/imageio.h>
#include <OpenImageIO/imageio.h>
#include <memory>
#include <cstring>
#include <execution>

namespace dakku {
bool writeImage(const std::filesystem::path &path, const float *rgb,
                int totalResX, int totalResY, int outBoundsMinX,
                int outBoundsMinY, int outBoundsMaxX, int outBoundsMaxY) {
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
}  // namespace dakku