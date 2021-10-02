//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_CAMERA_H_
#define DAKKU_INCLUDE_DAKKU_CORE_CAMERA_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>
#include <dakku/core/transform.h>
#include <memory>

namespace dakku {

struct CameraSample {
  Point2f pFilm;
};

class Camera {
 public:
  explicit Camera(const Transform &cameraToWorld, std::shared_ptr<Film> film);
  virtual ~Camera() = default;
  virtual Float generateRay(const CameraSample &sample, Ray &ray) const = 0;

  Transform cameraToWorld;
  std::shared_ptr<Film> film{nullptr};
};
}  // namespace dakku

#endif  // DAKKU_INCLUDE_DAKKU_CORE_CAMERA_H_
