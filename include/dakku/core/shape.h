//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_SHAPE_H_
#define DAKKU_INCLUDE_DAKKU_CORE_SHAPE_H_
#include <dakku/core/dakku.h>
#include <dakku/core/geometry.h>

namespace dakku {

class Shape {
 public:
  Shape() = default;
  virtual ~Shape() = default;
  [[nodiscard]] virtual Float area() const = 0;
  virtual Interaction sample(const Point2f &u, Float &pdf) const = 0;
  Interaction sample(const Interaction &ref, const Point2f &u,
                     Float &pdf) const;
};
}  // namespace dakku
#endif  // DAKKU_INCLUDE_DAKKU_CORE_SHAPE_H_
