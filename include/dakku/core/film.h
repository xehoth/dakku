//
// Created by xehoth on 2021/10/1.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_FILM_H_
#define DAKKU_INCLUDE_DAKKU_CORE_FILM_H_
#include "dakku.h"
#include "geometry.h"
#include "spectrum.h"
#include <vector>
#include <string>

namespace dakku {
class Film {
 public:
  explicit Film(const Point2i &resolution);

  const Point2i fullResolution;

  std::vector<RGBSpectrum> pixels;

  RGBSpectrum &getPixel(const Point2i &p) {
    const int width = fullResolution.x;
    const int offset = p.x + p.y * width;
    return pixels[offset];
  }

  void write(const std::string &fileName);
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_FILM_H_
