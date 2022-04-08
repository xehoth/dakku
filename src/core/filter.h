#ifndef DAKKU_CORE_FILTER_H_
#define DAKKU_CORE_FILTER_H_
#include <core/object.h>

namespace dakku {

/**
 * @brief filter class
 * all filters are centered at origin $(0, 0)$ and defined a radius
 * beyond which they have a value of $0$
 * this width may be different in the $x$ and $y$ directions
 */
class DAKKU_EXPORT_CORE Filter : public Object {
 public:
  DAKKU_DECLARE_OBJECT(Filter, Object);

  /**
   * @brief Construct a new Filter object
   * The filter's overall extent in each direction is twice the value given in
   * `radius`
   *
   * @param radius radius of the filter
   */
  explicit Filter(const Vector2f &radius);

  /**
   * @brief Given a 2D point which is relative to the center of the filter,
   * the filter's value at that point is returned.
   *
   * @param p the given 2D point
   * @return float filter's value
   */
  [[nodiscard]] virtual float evaluate(const Point2f &p) const = 0;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  /// filter radius
  const Vector2f radius;
  /// filter radius inversion
  const Vector2f invRadius;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};
}  // namespace dakku
#endif