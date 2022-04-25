#ifndef DAKKU_CORE_FILTER_H_
#define DAKKU_CORE_FILTER_H_
#include <core/vector.h>

namespace dakku {

class DAKKU_EXPORT_CORE Filter {
 public:
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

  /// filter radius
  const Vector2f radius;
  /// filter radius inversion
  const Vector2f inv_radius;
};
}  // namespace dakku
#endif