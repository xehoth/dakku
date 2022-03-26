#ifndef DAKKU_SRC_CORE_TRANSFORM_CACHE_H_
#define DAKKU_SRC_CORE_TRANSFORM_CACHE_H_
#include <core/singleton.h>
#include <core/sobject.h>
#include <core/transform.h>
#include <core/memory.h>
#include <unordered_map>
#include <unordered_set>
#include <string_view>

DAKKU_BEGIN

class DAKKU_EXPORT_CORE TransformCache final
    : public Singleton<TransformCache> {
 public:
  const Transform *lookUp(const Transform &t);
  void clear();

 private:
  friend class Singleton<TransformCache>;

  explicit TransformCache() = default;
  struct TransformCacheEqual {
    bool operator()(const Transform *a, const Transform *b) const noexcept {
      return a == b && *a == *b;
    }
  };

  struct TransformCacheHash {
    size_t operator()(const Transform *t) const noexcept {
      const char *ptr = reinterpret_cast<const char *>(&t->getMatrix());
      constexpr size_t size = sizeof(Matrix4x4);
      return std::hash<std::string_view>()(std::string_view{ptr, size});
    }
  };

  // transform cache
  std::unordered_set<const Transform *, TransformCacheHash, TransformCacheEqual>
      cache;

  TypedMemoryArena<Transform> arena;
};

DAKKU_END
#endif  // DAKKU_SRC_CORE_TRANSFORM_CACHE_H_
