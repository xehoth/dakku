#ifndef DAKKU_CAMERAS_FWD_H_
#define DAKKU_CAMERAS_FWD_H_
#include <core/camera.h>

namespace dakku {
#if DAKKU_BUILD_MODULE != DAKKU_CAMERAS_MODULE
#define DAKKU_EXPORT_CAMERAS DAKKU_IMPORT
#else
#define DAKKU_EXPORT_CAMERAS DAKKU_EXPORT
#endif
}  // namespace dakku
#endif