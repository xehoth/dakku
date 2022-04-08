#include <core/filter.h>

namespace dakku {

Filter::Filter(const Vector2f &radius)
    : radius(radius), invRadius(1.0f / radius) {}

Filter::Filter(const Property &p)
    : Filter(p.getVectorIf("radius", Vector2f{0.5f, 0.5f})) {}
}  // namespace dakku