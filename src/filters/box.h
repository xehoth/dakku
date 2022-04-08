#ifndef DAKKU_FILTERS_BOX_H_
#define DAKKU_FILTERS_BOX_H_
#include <filters/fwd.h>
#include <core/filter.h>

namespace dakku {

/**
 * @brief box filter  
 * the box filter is simple, just return $1$ for all values
 */
class DAKKU_EXPORT_FILTERS BoxFilter : public Filter {
 public:
  DAKKU_DECLARE_OBJECT(BoxFilter, Filter);
  using Filter::Filter;

  [[nodiscard]] float evaluate(const Point2f &p) const override;
};
}  // namespace dakku
#endif