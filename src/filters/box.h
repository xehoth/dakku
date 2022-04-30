#ifndef DAKKU_FILTERS_BOX_H_
#define DAKKU_FILTERS_BOX_H_
#include <filters/fwd.h>

namespace dakku {

/**
 * @brief box filter
 * the box filter is simple, just return $1$ for all values
 */
class DAKKU_EXPORT_FILTERS BoxFilter : public Filter {
 public:
  using Filter::Filter;

  [[nodiscard]] float evaluate(const Point2f &p) const override;
};

/**
 * @brief Create a box filter object
 * 
 */
DAKKU_EXPORT_FILTERS Filter *create_box_filter(float x_width, float y_width);

DAKKU_DECLARE_LUA_OBJECT(BoxFilter, DAKKU_EXPORT_FILTERS);
}  // namespace dakku
#endif