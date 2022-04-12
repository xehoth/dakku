#include <cameras/perspective.h>
#include <core/film.h>
#include <core/state.h>

namespace dakku {

PerspectiveCamera::PerspectiveCamera(const Transform &cameraToWorld,
                                     const Bounds2f &screenWindow, float fov,
                                     Film *film)
    : ProjectiveCamera(cameraToWorld, perspective(fov, 1e-2f, 1000.0f),
                       screenWindow, film) {
  dxCamera =
      (rasterToCamera(Point3f(1, 0, 0)) - rasterToCamera(Point3f(0, 0, 0)));
  dyCamera =
      (rasterToCamera(Point3f(0, 1, 0)) - rasterToCamera(Point3f(0, 0, 0)));
  // compute image plane bounds at $z = 1$ for PerspectiveCamera
  Point2i res = film->fullResolution;
  Point3f pMin = rasterToCamera(Point3f(0, 0, 0));
  Point3f pMax = rasterToCamera(Point3f(res.x(), res.y(), 0));
  pMin /= pMin.z();
  pMax /= pMax.z();
  A = std::abs((pMax.x() - pMin.x()) * (pMax.y() - pMin.y()));
}

Bounds2f getScreenWindow() {
  float frame = static_cast<float>(renderState.film->fullResolution.x()) /
                static_cast<float>(renderState.film->fullResolution.y());
  Bounds2f screen;
  if (frame > 1) {
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
  return screen;
}

PerspectiveCamera::PerspectiveCamera(const Property &p)
    : PerspectiveCamera(p["transform"].mergeTransform(), getScreenWindow(),
                        p.getNumberIf("fov", 90.0f), renderState.film.get()) {}

float PerspectiveCamera::generateRay(const CameraSample &sample,
                                     Ray &ray) const {
  // compute raster and camera sample positions
  Point3f pFilm = Point3f(sample.pFilm.x(), sample.pFilm.y(), 0);
  Point3f pCamera = rasterToCamera(pFilm);
  ray = Ray{Point3f(0, 0, 0), normalize(Vector3f(pCamera))};
  ray = cameraToWorld(ray);
  return 1;
}

float PerspectiveCamera::generateRayDifferential(const CameraSample &sample,
                                                 RayDifferential &ray) const {
  // compute raster and camera sample positions
  Point3f pFilm = Point3f(sample.pFilm.x(), sample.pFilm.y(), 0);
  Point3f pCamera = rasterToCamera(pFilm);
  Vector3f dir = normalize(Vector3f(pCamera.x(), pCamera.y(), pCamera.z()));
  ray = RayDifferential(Point3f(0, 0, 0), dir);

  ray.rxOrigin = ray.ryOrigin = ray.o;
  ray.rxDirection = normalize(Vector3f(pCamera) + dxCamera);
  ray.ryDirection = normalize(Vector3f(pCamera) + dyCamera);

  ray = cameraToWorld(ray);
  ray.hasDifferentials = true;
  return 1;
}
}  // namespace dakku