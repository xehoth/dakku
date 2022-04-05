#ifndef DAKKU_LOGGER_FWD_H_
#define DAKKU_LOGGER_FWD_H_
#include <platform.h>

namespace dakku {

#if DAKKU_BUILD_MODULE != DAKKU_LOGGER_MODULE
#define DAKKU_EXPORT_LOGGER DAKKU_IMPORT
#else
#define DAKKU_EXPORT_LOGGER DAKKU_EXPORT
#endif

class DAKKU_EXPORT_LOGGER Logger;
}
#endif