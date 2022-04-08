#include <core/filter.h>

namespace dakku {

Filter::Filter(const Vector2f &radius)
    : radius(radius), invRadius(1.0f / radius) {}

Filter::Filter(const Property &p) : Filter(Vector2f{p["radius"].getVector()}) {}
}  // namespace dakku