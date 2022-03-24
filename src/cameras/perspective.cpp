#include <cameras/perspective.h>

DAKKU_BEGIN
void PerspectiveCamera::construct(const Transform &cameraToWorld,
                                  const Bounds2f &screenWindow, Float fov,
                                  Film *film) {
  ProjectiveCamera::construct(cameraToWorld, perspective(fov, 1e-2f, 1000.0f),
                              screenWindow, film);
}

Float PerspectiveCamera::generateRay(const CameraSample &sample,
                                     Ray &ray) const {
  // compute raster and camera sample positions
  Point3f pFilm = Point3f(sample.pFilm.x(), sample.pFilm.y(), 0);
  Point3f pCamera = rasterToCamera(pFilm);
  ray = Ray(Point3f(0, 0, 0), Vector3f(pCamera).normalized());

  ray = cameraToWorld(ray);
  return 1;
}

void PerspectiveCamera::serialize(Json &json, OutputStream *) const {
  DAKKU_ERR("unimplemented");
}
void PerspectiveCamera::unserialize(const Json &json, InputStream *) {
  DAKKU_ERR("unimplemented");
}

DAKKU_END