#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string_view>
#include <span>

namespace dakku {

class Window {
 public:
  explicit Window(int width, int height, std::string_view title);
  [[nodiscard]] bool is_running() const;
  void update();
  ~Window();
  void draw(int w, int h, std::span<const float> pixels);

 private:
  GLFWwindow *window;
};

}  // namespace dakku::gui
