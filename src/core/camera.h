#ifndef DAKKU_CORE_CAMERA_H_
#define DAKKU_CORE_CAMERA_H_
#include <core/object.h>

namespace dakku {

/**
 * @brief camera
 *
 * film must be constructed before constructing camera
 *
 */
class DAKKU_EXPORT_CORE Camera : public Object {
 public:
  DAKKU_DECLARE_OBJECT(Camera, Object);

  explicit Camera(const Transform &cameraToWorld, Film *film);

  /**
   * @brief generate ray according to given sample
   *
   */
  virtual float generateRay(const CameraSample &sample, Ray &ray) const = 0;

  /**
   * @brief generate ray differential according to given sample
   *
   */
  virtual float generateRayDifferential(const CameraSample &sample,
                                        RayDifferential &rayD) const = 0;

  /// camera to world transform
  Transform cameraToWorld;
  /// film
  Film *film;
};

/**
 * @brief camera sample
 *
 */
struct CameraSample {
  /// film sample point
  Point2f pFilm;
};

class DAKKU_EXPORT_CORE ProjectiveCamera : public Camera {
 public:
  DAKKU_DECLARE_OBJECT(ProjectiveCamera, Camera);

  // explicit ProjectiveCamera(const Transform &cameraToWorld, const Transform &cameraToScreen, const Bounds2f &screenWindow)
};
}  // namespace dakku
#endif