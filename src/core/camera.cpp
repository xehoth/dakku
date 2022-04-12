#include <core/camera.h>
#include <core/state.h>

namespace dakku {

Camera::Camera(const Transform &cameraToWorld, Film *film)
    : cameraToWorld(cameraToWorld), film(film) {}

Camera::Camera(const Property &p)
    : Camera(p["transform"].mergeTransform(), renderState.film.get()) {}

}  // namespace dakku