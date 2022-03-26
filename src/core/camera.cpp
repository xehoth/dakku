#include <core/camera.h>

DAKKU_BEGIN
void Camera::construct(const Transform &_cameraToWorld, Film *_film) {
  this->cameraToWorld = _cameraToWorld;
  this->film = _film;
  if (cameraToWorld.hasScale()) {
    DAKKU_WARN("scaling detected in world-to-camera transformation");
  }
}

DAKKU_END