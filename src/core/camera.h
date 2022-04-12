#ifndef DAKKU_CORE_CAMERA_H_
#define DAKKU_CORE_CAMERA_H_
#include <core/object.h>

namespace dakku {

/**
 * @brief camera
 *
 */
class DAKKU_EXPORT_CORE Camera : public Object {
 public:
  DAKKU_DECLARE_OBJECT(Camera, Object);

  explicit Camera(const Transform &cameraToWorld, Film *film);

  /// camera to world transform
  Transform cameraToWorld;
  /// film
  Film *film;
};
}  // namespace dakku
#endif