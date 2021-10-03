//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/shape.h>
#include <dakku/core/interaction.h>
namespace dakku {

Interaction Shape::sample(const Interaction &ref, const Point2f &u,
                          Float &pdf) const {
  Interaction intr = this->sample(u, pdf);
  Vector3f wi = intr.p - ref.p;
  if (wi.lengthSquared() == 0) {
    pdf = 0;
  } else {
    wi = normalize(wi);
    pdf *= (ref.p - intr.p).lengthSquared() / absDot(intr.n, -wi);
    if (std::isinf(pdf)) pdf = 0;
  }
  return intr;
}
}  // namespace dakku