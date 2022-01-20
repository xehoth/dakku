#include <logger/logger.h>
#include <gui/window.h>
#include <cxxopts.hpp>

using namespace dakku;

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
  init_logger();
  if (args.count("gui")) {
    auto window = std::make_unique<Window>(640, 480, "dakku");
    std::vector<float> a(640 * 480 * 3);
    while (window->is_running()) {
      window->update();
      window->draw(640, 480, a);
    }
  }
  return 0;
}