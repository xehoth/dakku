#include <core/transform_cache.h>

DAKKU_BEGIN

const Transform *TransformCache::lookUp(const Transform &t) {
  if (auto it = cache.find(&t); it != cache.end()) return *it;
  Transform *pTrans = arena.allocObject(t);
  cache.insert(pTrans);
  return pTrans;
}

void TransformCache::clear() {
  cache.clear();
  arena.release();
}

DAKKU_END
