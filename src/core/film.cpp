//
// Created by xehoth on 2021/10/1.
//
#include <dakku/core/film.h>

namespace dakku {

Film::Film(const Point2i &resolution) : fullResolution(resolution) {
  this->pixels.resize(resolution.x * resolution.y);
}
}  // namespace dakku