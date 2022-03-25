#ifndef DAKKU_SRC_CORE_MEMORY_H_
#define DAKKU_SRC_CORE_MEMORY_H_
#include <core/fwd.h>
#include <core/logger.h>
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
  std::pmr::memory_resource *upStream;
};

template <typename T>
class TypedMemoryArena {
 public:
  template <typename... Args>
  T *allocObject(Args &&...args) {
    return allocator.template new_object<T>(std::forward<Args>(args)...);
  }
  void release() { resource.release(); }

 private:
  L1CacheLineAlignedResource upStream;
  std::pmr::unsynchronized_pool_resource resource{&upStream};
  std::pmr::polymorphic_allocator<T> allocator{&resource};
};

class MemoryArena {
 public:
  explicit MemoryArena() = default;
  explicit MemoryArena(std::pmr::memory_resource *buffer)
      : upStream(buffer), resource(&upStream) {}
  template <typename T, typename... Args>
  T *allocObject(Args &&...args) {
    return std::pmr::polymorphic_allocator<T>{&resource}.template new_object<T>(
        std::forward<Args>(args)...);
  }
  void release() { resource.release(); }

 private:
  L1CacheLineAlignedResource upStream;
  std::pmr::unsynchronized_pool_resource resource{&upStream};
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_MEMORY_H_
