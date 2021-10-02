//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CAMERAS_PERSPECTIVE_H_
#define DAKKU_INCLUDE_DAKKU_CAMERAS_PERSPECTIVE_H_
#include <dakku/core/camera.h>

namespace dakku {

/**
 * Perspective camera
 *
 * Camera coordinate (only for dir calculation):
 * The virtual image plane's distance (focal distance) is 1
 * Calculate w, h with fov
 *
 * Raster coordinate: (0, 0) is up-left, (w, h) = resolution
 */
class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera(const Transform &cameraToWorld, Float fov,
                    std::shared_ptr<Film> film);

  Float generateRay(const CameraSample &sample, Ray &ray) const override;

 private:
  [[nodiscard]] Vector3f rasterToCamera(const Point2f &p) const;

  Float imagePlaneWidth;
  Float imagePlaneHeight;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CAMERAS_PERSPECTIVE_H_
