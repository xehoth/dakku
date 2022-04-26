#ifndef DAKKU_CORE_MEMORY_H_
#define DAKKU_CORE_MEMORY_H_
#include <core/logger.h>

#include <oneapi/tbb.h>
#include <oneapi/tbb/scalable_allocator.h>

#if __has_include(<malloc.h>)
#include <malloc.h>
#endif
#if __has_include(<alloca.h>)
#include <alloca.h>
#endif
#include <memory_resource>

namespace dakku {

/**
 * @brief allocated memory from stack
 * @param type type that needs to allocate
 * @param count the number of [type]s
 */
#define DAKKU_ALLOCA(type, count) \
  (reinterpret_cast<(type) *>(alloca((count) * sizeof(type))))

/**
 * @brief memory arena (unsynchronized)
 *
 */
class MemoryArena {
 public:
  explicit MemoryArena() = default;
  explicit MemoryArena(std::pmr::memory_resource *buffer)
      : upStream(buffer), resource(&upStream) {}

  /**
   * @brief allocate and construct a object from the arena
   *
   * @tparam T object type
   * @tparam Args argument types
   * @param args arguments
   * @return T* object instance
   */
  template <typename T, typename... Args>
  T *allocObject(Args &&...args) {
    return std::pmr::polymorphic_allocator<T>{&resource}.template new_object<T>(
        std::forward<Args>(args)...);
  }

  /**
   * @brief release the memory arena
   *
   */
  void release() { resource.release(); }

 private:
  /// cache aligned scalable resource
  oneapi::tbb::cache_aligned_resource upStream{
      oneapi::tbb::scalable_memory_resource()};
  /// cache aligned scalable resource
  std::pmr::unsynchronized_pool_resource resource{&upStream};
};

/**
 * @brief global memory arena
 *
 */
class GlobalMemoryArena {
 public:
  /**
   * @brief get memory arena instance
   *
   */
  static MemoryArena &instance();

 private:
  GlobalMemoryArena() = default;
  /// memory arena
  MemoryArena arena;
};
}  // namespace dakku
#endif