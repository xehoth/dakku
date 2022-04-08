#include <filters/triangle.h>

namespace dakku {

TriangleFilter::TriangleFilter(const Property &p) : Filter(p) {}

float TriangleFilter::evaluate(const Point2f &p) const {
  return std::max(0.0f, radius.x() - std::abs(p.x())) *
         std::max(0.0f, radius.y() - std::abs(p.y()));
}
}  // namespace dakku