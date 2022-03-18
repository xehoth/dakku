#ifndef DAKKU_SRC_CORE_MEMORY_H_
#define DAKKU_SRC_CORE_MEMORY_H_
#include <core/fwd.h>
#if __has_include(<malloc.h>)
#include <malloc.h>
#endif
#if __has_include(<alloca.h>)
#include <alloca.h>
#endif

DAKKU_BEGIN

#define DAKKU_ALLOCA(type, count) \
  (reinterpret_cast<type *>(alloca((count) * sizeof(type))))

DAKKU_END
#endif  // DAKKU_SRC_CORE_MEMORY_H_
