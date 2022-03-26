#ifndef DAKKU_SRC_CAMERAS_PERSPECTIVE_H_
#define DAKKU_SRC_CAMERAS_PERSPECTIVE_H_
#include <core/camera.h>
#include <core/vector.h>

DAKKU_BEGIN
class PerspectiveCamera : public ProjectiveCamera {
 public:
  DAKKU_DECLARE_OBJECT(PerspectiveCamera);

  void serialize(Json &json, OutputStream *) const override;
  void unserialize(const Json &json, InputStream *) override;

  void construct(const Transform &cameraToWorld, const Bounds2f &screenWindow,
                 Float fov, Film *film);
  Float generateRay(const CameraSample &sample, Ray &ray) const override;
};
DAKKU_END

#endif  // DAKKU_SRC_CAMERAS_PERSPECTIVE_H_
