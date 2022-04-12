#include <core/transform_cache.h>
#include <core/property.h>
#include <core/logger.h>

namespace dakku {

TransformCache &TransformCache::instance() {
  static TransformCache _instance;
  return _instance;
}

const Transform *TransformCache::lookUp(const Transform &t) {
  if (auto it = cache.find(&t); it != cache.end()) return *it;
  auto *pTrans = arena.allocObject<Transform>(t);
  cache.insert(pTrans);
  return pTrans;
}

void TransformCache::clear() {
  cache.clear();
  arena.release();
}

const Transform *TransformCache::lookUpByProperty(const Property &p) {
  if (p.isTransformType()) return lookUp(p.getTransform());
  DAKKU_CHECK(p.isObjectType() && p.getObject().contains("transform"),
              "property should be a object containing 'transform': {}", p);
  return lookUp(p["transform"].mergeTransform());
}
}  // namespace dakku