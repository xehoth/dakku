#include <core/api.h>
#include <filters/api.h>
#include <shapes/api.h>
#include <core/transform.h>
#include <cxxopts.hpp>
#include <iostream>
using namespace dakku;

int main(int argc, const char *argv[]) {
  core::init();
  filters::init();
  shapes::init();
//  Json json = Json::parse("[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]");
//  Matrix4x4 a = json;
//  std::cout << a << std::endl;
//  Json out = a;
//  std::cout << out << std::endl;
//  a = out;
//  std::cout << a;
  renderState.load("../../../assets/scene.json");
//  renderState.save("../../../assets/scene_bak.json");
//  auto state = RenderState::load("../../../assets/scene.json");
//  state->save("../../../assets/scene_bak.json");
//  std::cout << state->film.getSampleBounds();
  // Class a;
  //   cxxopts::Options options("dakku", "renderer");
  //   options.add_options()("s,scene", "scene file",
  //   cxxopts::value<std::string>()); options.add_options()("h,help", "print
  //   help"); options.add_options()("g,gui", "show render result in gui"); auto
  //   args = options.parse(argc, argv); if (args.count("help") ||
  //   args.arguments().empty()) {
  //     std::cout << options.help() << std::endl;
  //     return 0;
  //   }
  ////  init_logger();
  //  if (args.count("gui")) {
  ////    auto window = std::make_unique<Window>(640, 480, "dakku");
  ////    std::vector<float> a(640 * 480 * 3);
  ////    while (window->is_running()) {
  ////      window->update();
  ////      window->draw(640, 480, a);
  ////    }
  //  }
  return 0;
}