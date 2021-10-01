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
  if (ext == "png") {
    stbi_write_png(fileName.c_str(), fullResolution.x, fullResolution.y, 3,
                   pixels.data(), 0);
  }
}
}  // namespace dakku