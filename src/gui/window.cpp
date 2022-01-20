#include <gui/window.h>
#include <logger/logger.h>

namespace dakku {

Window::Window(int width, int height, std::string_view title) {
  LOG_INFO("Create window of ({} x {})", width, height);
  DAKKU_CHECK(glfwInit() == GLFW_TRUE, "Failed to init_logger glfw");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
  DAKKU_CHECK(window != nullptr, "Failed to create window");
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  DAKKU_CHECK(glewInit() == GLEW_OK, "Failed to init_logger glew");
}

Window::~Window() { glfwTerminate(); }

bool Window::is_running() const { return !glfwWindowShouldClose(window); }

void Window::update() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  glfwPollEvents();
  glfwSwapBuffers(window);
}

void Window::draw(int w, int h, std::span<const float> pixels) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  DAKKU_CHECK(pixels.size() == static_cast<size_t>(w) * h * 3, "Wrong buffer size");
  glDrawPixels(w, h, GL_RGB, GL_FLOAT, pixels.data());
}
}  // namespace dakku::gui
