#include <cameras/perspective.h>
#include <core/film.h>
#include <core/state.h>
#include <core/transform.h>

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
  Float fov = 0;
  if (json.contains("fov")) {
    DAKKU_UNSER_J(fov);
  } else {
    DAKKU_ERR("perspective camera requires fov");
  }
  Film *film = &renderState.film;
  Float frame = static_cast<Float>(film->fullResolution.x()) /
                static_cast<Float>(film->fullResolution.y());
  Bounds2f screen;
  if (frame > 1.f) {
    screen.pMin.x() = -frame;
    screen.pMax.x() = frame;
    screen.pMin.y() = -1;
    screen.pMax.y() = 1;
  } else {
    screen.pMin.x() = -1;
    screen.pMax.x() = 1;
    screen.pMin.y() = -1 / frame;
    screen.pMax.y() = 1 / frame;
  }
  Transform cameraToWorld;
  if (json.contains("transform")) {
    Matrix4x4 transform;
    json.at("transform").get_to(transform);
    transform = inverse(transpose(transform));
    cameraToWorld = Transform(transform);
    DAKKU_DEBUG("transform: {}", transform);
  }
  construct(cameraToWorld, screen, fov, film);
}

DAKKU_END