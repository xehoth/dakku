#ifndef DAKKU_STREAM_FWD_H_
#define DAKKU_STREAM_FWD_H_
#include <core/serialization.h>

namespace dakku {
#if DAKKU_BUILD_MODULE != DAKKU_STREAM_MODULE
#define DAKKU_EXPORT_STREAM DAKKU_IMPORT
#else
#define DAKKU_EXPORT_STREAM DAKKU_EXPORT
#endif
}  // namespace dakku
#endif