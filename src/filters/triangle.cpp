#include <filters/triangle.h>

namespace dakku {

float TriangleFilter::evaluate(const Point2f &p) const {
  return std::max(0.0f, radius.x() - std::abs(p.x())) *
         std::max(0.0f, radius.y() - std::abs(p.y()));
}

DAKKU_IMPLEMENT_LUA_OBJECT(TriangleFilter, [] {
  DAKKU_INFO("register TriangleFilter");
  auto &state = Lua::instance().get_state();
  state.new_usertype<TriangleFilter>(
      "TriangleFilter", sol::constructors<TriangleFilter(const Vector2f &)>());
  return 0;
});
}  // namespace dakku