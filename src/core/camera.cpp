//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/camera.h>

#include <utility>

namespace dakku {

Camera::Camera(const Transform &cameraToWorld, std::shared_ptr<Film> film)
    : cameraToWorld(cameraToWorld), film(std::move(film)) {}
}  // namespace dakku