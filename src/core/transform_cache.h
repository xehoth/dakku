#include <core/transform.h>
#include <core/memory.h>
#include <unordered_set>

namespace dakku {

class DAKKU_EXPORT_CORE TransformCache final {
 public:
 /**
  * @brief get transform cache instance
  * 
  */
  static TransformCache &instance();

  /**
   * @brief look up a transform in cache
   * 
   */
  const Transform *lookUp(const Transform &t);


  /**
   * @brief clear the cache
   * 
   */
  void clear();

  /**
   * @brief get a transform by property
   * 
   */
  const Transform *lookUpByProperty(const Property &p);

 private:
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


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  /// transform cache
  std::unordered_set<const Transform *, TransformCacheHash, TransformCacheEqual>
      cache;

  MemoryArena arena;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};
}  // namespace dakku