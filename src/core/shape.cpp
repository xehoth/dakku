#include <core/shape.h>
#include <core/interaction.h>

DAKKU_BEGIN

void Shape::construct(const Transform *_objectToWorld,
                      const Transform *_worldToObject) {
  this->objectToWorld = _objectToWorld;
  this->worldToObject = _worldToObject;
  this->transformSwapsHandedness = objectToWorld->swapsHandedNess();
}

Interaction Shape::sample(const Interaction &ref, const Point2f &u,
                          Float &pdf) const {
  Interaction it = sample(u, pdf);
  Vector3f wi = it.p - ref.p;
  if (wi.squaredNorm() == 0) {
    pdf = 0;
  } else {
    wi.normalize();
    pdf *= (ref.p - it.p).squaredNorm() / it.n.absDot(-wi);
    if (std::isinf(pdf)) pdf = 0;
  }
  return it;
}

Float Shape::pdf(const Interaction &ref, const Interaction &lightIt,
                 const Vector3f &wi) const {
  // convert light sample weight to solid angle measure
  Float pdf =
      (ref.p - lightIt.p).squaredNorm() / (lightIt.n.absDot(-wi) * area());
  if (std::isinf(pdf)) pdf = 0;
  return pdf;
}

DAKKU_END
