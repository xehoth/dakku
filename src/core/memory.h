#ifndef DAKKU_CORE_MEMORY_H_
#define DAKKU_CORE_MEMORY_H_
#include <core/logger.h>
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
  (reinterpret_cast<type *>(alloca((count) * sizeof(type))))

/**
 * @brief memory resource that at least aligned with L1 cache line size
 */
class L1CacheLineAlignedResource : public std::pmr::memory_resource {
 public:
  explicit L1CacheLineAlignedResource(
      std::pmr::memory_resource *upStream = std::pmr::get_default_resource())
      : upStream(upStream) {}

  void *do_allocate(size_t bytes, size_t align) override {
    DAKKU_DEBUG("allocate pool of {} bytes", bytes);
    return upStream->allocate(bytes, std::max(align, L1_CACHE_LINE_SIZE));
  }

  void do_deallocate(void *ptr, size_t bytes, size_t align) override {
    DAKKU_DEBUG("deallocate pool of {} bytes", bytes);
    upStream->deallocate(ptr, bytes, std::max(align, L1_CACHE_LINE_SIZE));
  }

  [[nodiscard]] bool do_is_equal(
      const memory_resource &rhs) const noexcept override {
    return this == &rhs;
  }

 private:
  /// upstream memory resource
  std::pmr::memory_resource *upStream;
};

/**
 * @brief memory arena
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
  /// upstream resource l1 cache line aligned
  L1CacheLineAlignedResource upStream;
  /// pool resource
  std::pmr::unsynchronized_pool_resource resource{&upStream};
};
}  // namespace dakku
#endif