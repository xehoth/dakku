#ifndef DAKKU_FILTERS_Triangle_H_
#define DAKKU_FILTERS_Triangle_H_
#include <filters/fwd.h>

namespace dakku {
/**
 * @brief triangle filter
 * the weight falls off linearly from the filter center over the square extent
 * of the filter
 */
class DAKKU_EXPORT_FILTERS TriangleFilter : public Filter {
 public:
  using Filter::Filter;

  [[nodiscard]] float evaluate(const Point2f &p) const override;
};

DAKKU_DECLARE_LUA_OBJECT(TriangleFilter, DAKKU_EXPORT_FILTERS);
}  // namespace dakku
#endif