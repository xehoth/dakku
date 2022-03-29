#ifndef DAKKU_SRC_CORE_FILTER_H_
#define DAKKU_SRC_CORE_FILTER_H_
#include <core/sobject.h>
#include <core/vector.h>

DAKKU_BEGIN
/**
 * dakku Filter
 */
class Filter : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Filter, SerializableObject);
  /**
   * construct the filter with given radius
   * @param _radius the given radius of the filter
   */
  void construct(const Vector2f &_radius) {
    radius = _radius;
    invRadius = Vector2f(1 / _radius.x(), 1 / _radius.y());
  }
  /**
   * evaluate the filter weight at the given point
   * @param p the point to evaluate
   * @return the filter weight
   */
  [[nodiscard]] virtual Float evaluate(const Point2f &p) const = 0;
  
  Vector2f radius, invRadius;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_FILTER_H_
