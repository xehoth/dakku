#ifndef DAKKU_SRC_CAMERAS_PERSPECTIVE_H_
#define DAKKU_SRC_CAMERAS_PERSPECTIVE_H_
#include <cameras/fwd.h>

namespace dakku {
/**
 * @brief perspective camera
 *
 */
class DAKKU_EXPORT_CAMERAS PerspectiveCamera : public ProjectiveCamera {
 public:
  DAKKU_DECLARE_OBJECT(PerspectiveCamera, ProjectiveCamera);

  /**
   * @brief Construct a new Perspective Camera object
   *
   * @param screenWindow screen bounds
   * @param fov vertical fov
   */
  explicit PerspectiveCamera(const Transform &cameraToWorld,
                             const Bounds2f &screenWindow, float fov,
                             Film *film);
  explicit PerspectiveCamera(const Property &p);

  float generateRay(const CameraSample &sample, Ray &ray) const override;

  float generateRayDifferential(const CameraSample &sample,
                                RayDifferential &rayD) const override;

 private:
  /// differential changes in origin
  Vector3f dxCamera, dyCamera;
  float A;
};
}  // namespace dakku

#endif  // DAKKU_SRC_CAMERAS_PERSPECTIVE_H_
