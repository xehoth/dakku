#ifndef DAKKU_SRC_CAMERAS_API_H_
#define DAKKU_SRC_CAMERAS_API_H_
#include <core/class.h>
#include <cameras/perspective.h>

DAKKU_BEGIN
namespace cameras {
void init() { DAKKU_REGISTER_CLASS(PerspectiveCamera); }
}  // namespace cameras
DAKKU_END

#endif  // DAKKU_SRC_CAMERAS_API_H_
