#include <filters/box.h>

namespace dakku {

float BoxFilter::evaluate(const Point2f &) const { return 1.0f; }

DAKKU_IMPLEMENT_LUA_OBJECT(BoxFilter, [] {
  DAKKU_INFO("register BoxFilter");
  auto &state = Lua::instance().get_state();
  state.new_usertype<BoxFilter>(
      "BoxFilter", sol::constructors<BoxFilter(const Vector2f &)>());
  return 0;
});
}  // namespace dakku