#include <filters/box.h>
#include <core/memory.h>

namespace dakku {

float BoxFilter::evaluate(const Point2f &) const { return 1.0f; }

Filter *create_box_filter(float x_width, float y_width) {
  return GlobalMemoryArena::instance().allocObject<BoxFilter>(
      Vector2f(x_width, y_width));
}

DAKKU_IMPLEMENT_LUA_OBJECT(BoxFilter, [] {
  DAKKU_INFO("register BoxFilter");
  auto &state = Lua::instance().get_state();
  state.new_usertype<BoxFilter>(
      "BoxFilter", sol::constructors<BoxFilter(const Vector2f &)>());
  state.set_function("_create_box_filter", &create_box_filter);
  return 0;
});
}  // namespace dakku