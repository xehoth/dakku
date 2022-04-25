#include <core/filter.h>

namespace dakku {

Filter::Filter(const Vector2f &radius)
    : radius(radius), inv_radius(1.0f / radius) {}
}  // namespace dakku