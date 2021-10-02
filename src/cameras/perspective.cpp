//
// Created by xehoth on 2021/10/2.
//
#include <dakku/cameras/perspective.h>
#include <dakku/core/film.h>

namespace dakku {

PerspectiveCamera::PerspectiveCamera(const Transform &cameraToWorld, Float fov,
                                     std::shared_ptr<Film> film)
    : Camera(cameraToWorld, film) {
  const Float virtualFocalDistance = 1;
  Float halfFov = fov / 2;
  Float aspectRatio = film->getAspectRatio();
  Float scale = virtualFocalDistance * std::tan(radians(halfFov));
  this->imagePlaneWidth = scale * aspectRatio;
  this->imagePlaneHeight = scale;
}
Float PerspectiveCamera::generateRay(const CameraSample &sample,
                                     Ray &ray) const {
  Vector3f cameraDir = rasterToCamera(sample.pFilm);
  ray = cameraToWorld(Ray(Point3f(0), cameraDir));
  return 1;
}

Vector3f PerspectiveCamera::rasterToCamera(const Point2f &p) const {
  Float cameraX = (2 * p.x / static_cast<Float>(film->fullResolution.x) - 1) *
                  imagePlaneWidth;
  Float cameraY = (2 * p.y / static_cast<Float>(film->fullResolution.y) - 1) *
                  imagePlaneHeight;
  return Vector3f{cameraX, cameraY, static_cast<Float>(-1)};
}
}  // namespace dakku