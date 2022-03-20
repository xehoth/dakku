#ifndef DAKKU_SRC_CORE_MEMORY_H_
#define DAKKU_SRC_CORE_MEMORY_H_
#include <core/fwd.h>
#if __has_include(<malloc.h>)
#include <malloc.h>
#endif
#if __has_include(<alloca.h>)
#include <alloca.h>
#endif
#include <memory_resource>

DAKKU_BEGIN

#define DAKKU_ALLOCA(type, count) \
  (reinterpret_cast<type *>(alloca((count) * sizeof(type))))

template <typename T>
class TypedMemoryArena {
 public:
  template <typename... Args>
  T *allocObject(Args &&...args) {
    return allocator.template new_object<T>(std::forward<Args>(args)...);
  }
  void release() {
    resource.release();
  }

 private:
  std::pmr::unsynchronized_pool_resource resource;
  std::pmr::polymorphic_allocator<T> allocator{&resource};
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_MEMORY_H_
