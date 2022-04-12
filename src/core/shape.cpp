#include <core/shape.h>
#include <core/transform.h>
#include <core/transform_cache.h>
#include <core/interaction.h>

namespace dakku {

Shape::Shape(const Transform *objectToWorld, const Transform *worldToObject)
    : objectToWorld(objectToWorld),
      worldToObject(worldToObject),
      transformSwapsHandedness(objectToWorld->swapsHandedNess()) {}

Shape::Shape(const Transform *objectToWorld)
    : objectToWorld(objectToWorld),
      worldToObject(TransformCache::instance().lookUp(inverse(*objectToWorld))),
      transformSwapsHandedness(objectToWorld->swapsHandedNess()) {}

Shape::Shape(const Property &p)
    : Shape(TransformCache::instance().lookUpByProperty(p)) {}

bool Shape::hasIntersection(const Ray &ray, bool testAlphaTexture) const {
  return intersect(ray, nullptr, nullptr, testAlphaTexture);
}

Bounds3f Shape::worldBound() const { return (*objectToWorld)(objectBound()); }

float Shape::pdfPosition(const Interaction &) const { return 1 / area(); }

Interaction Shape::sampleDirection(const Interaction &ref, const Point2f &u,
                                   float &pdf) const {
  Interaction x = samplePosition(u, pdf);
  Vector3f wi = x.p - ref.p;
  if (wi.squaredNorm() == 0) {
    pdf = 0;
  } else {
    wi.normalize();
    pdf *= (ref.p - x.p).squaredNorm() / absDot(x.n, -wi);
    if (std::isinf(pdf)) pdf = 0;
  }
  return x;
}

float Shape::pdfDirection(const Interaction &ref, const Vector3f &wi) const {
  Ray ray = ref.spawnRay(wi);
  float tHit;
  SurfaceInteraction isectLight;
  if (!intersect(ray, &tHit, &isectLight, false)) return 0;

  float pdf = (ref.p - isectLight.p).squaredNorm() /
              (absDot(isectLight.n, -wi) * pdfPosition(isectLight));
  if (std::isinf(pdf)) pdf = 0;
  return pdf;
}
}  // namespace dakku