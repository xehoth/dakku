#ifndef DAKKU_SRC_CORE_FILTER_H_
#define DAKKU_SRC_CORE_FILTER_H_
#include <core/sobject.h>
#include <core/vector.h>

DAKKU_BEGIN
class Filter : public SerializableObject {
 public:
  void construct(const Vector2f &_radius) {
    radius = _radius;
    invRadius = Vector2f(1 / _radius.x(), 1 / _radius.y());
  }
  [[nodiscard]] virtual Float evaluate(const Point2f &p) const = 0;
  Vector2f radius, invRadius;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_FILTER_H_
