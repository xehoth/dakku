#ifndef DAKKU_SRC_CORE_CAMERA_H_
#define DAKKU_SRC_CORE_CAMERA_H_
#include <core/sobject.h>
#include <core/vector.h>
#include <core/bounds.h>
#include <core/transform.h>
#include <core/film.h>

DAKKU_BEGIN
class Camera : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Camera);

  void construct(const Transform &cameraToWorld, Film *film);
  virtual Float generateRay(const CameraSample &sample, Ray &ray) const = 0;
  Transform cameraToWorld;
  Film *film{};
};

struct CameraSample {
  Point2f pFilm;
};

class ProjectiveCamera : public Camera {
 public:
  DAKKU_DECLARE_OBJECT(ProjectiveCamera);

  void construct(const Transform &_cameraToWorld,
                 const Transform &_cameraToScreen, const Bounds2f &screenWindow,
                 Film *film) {
    Camera::construct(_cameraToWorld, film);
    this->cameraToScreen = _cameraToScreen;
    screenToRaster =
        scale(film->fullResolution.x(), film->fullResolution.y(), 1) *
        scale(1 / (screenWindow.pMax.x() - screenWindow.pMin.x()),
              1 / (screenWindow.pMin.y() - screenWindow.pMax.y()), 1) *
        translate(Vector3f(-screenWindow.pMin.x(), -screenWindow.pMax.y(), 0));
    rasterToScreen = inverse(screenToRaster);
    rasterToCamera = inverse(cameraToScreen) * rasterToScreen;
  }

 protected:
  Transform cameraToScreen, rasterToCamera;
  Transform screenToRaster, rasterToScreen;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_CAMERA_H_
