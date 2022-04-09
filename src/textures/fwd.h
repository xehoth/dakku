#ifndef DAKKU_TEXTURES_FWD_H_
#define DAKKU_TEXTURES_FWD_H_
#include <core/fwd.h>
#include <core/texture.h>

namespace dakku {
#if DAKKU_BUILD_MODULE != DAKKU_TEXTURES_MODULE
#define DAKKU_EXPORT_TEXTURES DAKKU_IMPORT
#else
#define DAKKU_EXPORT_TEXTURES DAKKU_EXPORT
#endif
}  // namespace dakku
#endif