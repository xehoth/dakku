//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/shape.h>
#include <dakku/core/interaction.h>
namespace dakku {

Interaction Shape::sample(const Interaction &ref, const Point2f &u,
                          Float &pdf) const {
  return Interaction{};
}
}