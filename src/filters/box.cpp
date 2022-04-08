#include <filters/box.h>

namespace dakku {

BoxFilter::BoxFilter(const Property &p) : Filter(p) {}

float BoxFilter::evaluate(const Point2f &) const { return 1.0f; }
}  // namespace dakku