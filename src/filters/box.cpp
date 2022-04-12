#include <filters/box.h>

namespace dakku {

BoxFilter::BoxFilter(const Property &p)
    : Filter(p.getVectorIf("radius", Vector2f{0.5f, 0.5f})) {}

float BoxFilter::evaluate(const Point2f &) const { return 1.0f; }
}  // namespace dakku