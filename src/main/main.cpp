#include <logger/logger.h>
#include <gui/window.h>
#include <cxxopts.hpp>
#include <string>
#include <iostream>
#include <core/geometry.h>

using namespace dakku;
using namespace dakku::core;

int main(int argc, const char *argv[]) {
  cxxopts::Options options("dakku", "renderer");
  options.add_options()("s,scene", "scene file", cxxopts::value<std::string>());
  options.add_options()("h,help", "print help");
  options.add_options()("g,gui", "show render result in gui");
  auto args = options.parse(argc, argv);
  if (args.count("help") || args.arguments().empty()) {
    std::cout << options.help() << std::endl;
    return 0;
  }
  log::init();
  if (args.count("gui")) {
    auto window = std::make_unique<gui::Window>(640, 480, "dakku");
    std::vector<float> a(640 * 480 * 3);
    while (window->is_running()) {
      window->update();
      window->draw(640, 480, a);
    }
  }
  Vector3f a, b, c;
  coordinate_system(a, b, c);
  return 0;
}