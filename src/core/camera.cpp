//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/camera.h>

namespace dakku {

Camera::Camera(const Transform &cameraToWorld, std::shared_ptr<Film> film)
    : cameraToWorld(cameraToWorld), film(film) {}
}  // namespace dakku