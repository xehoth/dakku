//
// Created by xehoth on 2021/10/12.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_IMAGE_H_
#define DAKKU_INCLUDE_DAKKU_CORE_IMAGE_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>
#include <memory>

namespace dakku {

enum class ImageWrap { REPEAT, BLACK, CLAMP };

template <typename T>
class Image {
 public:
  Image(const Point2i &resolution, const T *data,
        ImageWrap wrapMode = ImageWrap::REPEAT);
  [[nodiscard]] int width() const { return resolution.x; }
  [[nodiscard]] int height() const { return resolution.y; }
  T lookUp(const Point2f &st) const {
    Float s = st.s * resolution.s - static_cast<Float>(0.5);
    Float t = st.t * resolution.t - static_cast<Float>(0.5);
    int s0 = std::floor(s), t0 = std::floor(t);
    Float ds = s - static_cast<Float>(s0), dt = t - static_cast<Float>(t0);
    return (1 - ds) * (1 - dt) * texel(s0, t0) +
           (1 - ds) * dt * texel(s0, t0 + 1) +
           ds * (1 - dt) * texel(s0 + 1, t0) + ds * dt * texel(s0 + 1, t0 + 1);
  }

 private:
  [[nodiscard]] Float clamp(Float v) const {
    return glm::clamp(v, static_cast<Float>(0), INF);
  }

  T texel(int s, int t) const {
    switch (wrapMode) {
      case ImageWrap::REPEAT: {
        s %= resolution.s;
        t %= resolution.t;
        break;
      }
      case ImageWrap::BLACK: {
        if (s < 0 || s >= resolution.s || t < 0 || t >= resolution.t)
          return T{0};
        break;
      }
      case ImageWrap::CLAMP: {
        s = glm::clamp(s, 0, resolution.s - 1);
        t = glm::clamp(t, 0, resolution.t - 1);
        break;
      }
    }
    return image[t * resolution.s + s];
  }

  Point2i resolution;
  const ImageWrap wrapMode;
  std::unique_ptr<T[]> image;
};

template <typename T>
Image<T>::Image(const Point2i &resolution, const T *data, ImageWrap wrapMode)
    : resolution(resolution), wrapMode(wrapMode) {
  image.reset(new T[resolution.s * resolution.t]);
  std::copy(data, data + resolution.s * resolution.t, image.get());
}
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_IMAGE_H_
