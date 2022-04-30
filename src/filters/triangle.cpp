#include <filters/triangle.h>
#include <core/memory.h>

namespace dakku {

float TriangleFilter::evaluate(const Point2f &p) const {
  return std::max(0.0f, radius.x() - std::abs(p.x())) *
         std::max(0.0f, radius.y() - std::abs(p.y()));
}

Filter *create_triangle_filter(float x_width, float y_width) {
  return GlobalMemoryArena::instance().allocObject<TriangleFilter>(
      Vector2f(x_width, y_width));
}

DAKKU_IMPLEMENT_LUA_OBJECT(TriangleFilter, [] {
  DAKKU_INFO("register TriangleFilter");
  auto &state = Lua::instance().get_state();
  state.new_usertype<TriangleFilter>(
      "TriangleFilter", sol::constructors<TriangleFilter(const Vector2f &)>());
  state.set_function("_create_triangle_filter", &create_triangle_filter);
  return 0;
});
}  // namespace dakku