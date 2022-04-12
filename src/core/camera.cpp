#include <core/camera.h>
#include <core/state.h>

namespace dakku {

Camera::Camera(const Transform &cameraToWorld, Film *film)
    : cameraToWorld(cameraToWorld), film(film) {}
}  // namespace dakku