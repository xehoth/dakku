#ifndef DAKKU_CORE_FWD_H_
#define DAKKU_CORE_FWD_H_
#include <platform.h>

#if DAKKU_BUILD_MODULE != DAKKU_CORE_MODULE
#define DAKKU_EXPORT_CORE DAKKU_IMPORT
#else
#define DAKKU_EXPORT_CORE DAKKU_EXPORT
#endif

DAKKU_EXPORT_CORE void test();
#endif