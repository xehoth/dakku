#ifndef DAKKU_FILTERS_FWD_H_
#define DAKKU_FILTERS_FWD_H_
#include <core/serialization.h>

namespace dakku {
#if DAKKU_BUILD_MODULE != DAKKU_FILTERS_MODULE
#define DAKKU_EXPORT_FILTERS DAKKU_IMPORT
#else
#define DAKKU_EXPORT_FILTERS DAKKU_EXPORT
#endif
}  // namespace dakku
#endif