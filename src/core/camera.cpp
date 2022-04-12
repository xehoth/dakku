#include <core/camera.h>
#include <core/state.h>
#include <core/film.h>

namespace dakku {

Camera::Camera(const Transform &cameraToWorld, Film *film)
    : cameraToWorld(cameraToWorld), film(film) {}

ProjectiveCamera::ProjectiveCamera(const Transform &cameraToWorld,
                                   const Transform &cameraToScreen,
                                   const Bounds2f &screenWindow, Film *film)
    : Camera(cameraToWorld, film), cameraToScreen(cameraToScreen) {
  screenToRaster =
      // scale to raster size
      scale(static_cast<float>(film->fullResolution.x()),
            static_cast<float>(film->fullResolution.y()), 1.0f) *
      // scale to $[0, 1] ^ 2$ and flip y
      scale(1.0f / screenWindow.diagonal().x(),
            -1.0f / screenWindow.diagonal().y(), 1.0f) *
      // move screen to origin
      translate(Vector3f{-screenWindow.pMin.x(), -screenWindow.pMin.y(), 0});
  rasterToScreen = inverse(screenToRaster);
  // raster -> camera = raster -> screen => screen -> camera
  rasterToCamera = inverse(cameraToScreen) * rasterToScreen;
}
}  // namespace dakku