//
// Created by xehoth on 2021/10/1.
//
#include <dakku/core/film.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace dakku {

Film::Film(const Point2i &resolution) : fullResolution(resolution) {
  this->pixels.resize(resolution.x * resolution.y);
}

void Film::write(const std::string &fileName) {
  const std::string ext = fileName.substr(fileName.rfind('.') + 1);
  std::vector<std::uint8_t> output(this->pixels.size() * 3);
  for (size_t i = 0; i < this->pixels.size(); ++i) {
    output[i * 3 + 0] = static_cast<std::uint8_t>(this->pixels[i].r * 255.999);
    output[i * 3 + 1] = static_cast<std::uint8_t>(this->pixels[i].g * 255.999);
    output[i * 3 + 2] = static_cast<std::uint8_t>(this->pixels[i].b * 255.999);
  }
  if (ext == "png") {
    stbi_write_png(fileName.c_str(), fullResolution.x, fullResolution.y, 3,
                   output.data(), 0);
  }
}
}  // namespace dakku