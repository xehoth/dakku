//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_PARSER_H_
#define DAKKU_INCLUDE_DAKKU_CORE_PARSER_H_
#include <dakku/core/dakku.h>
#include <string>
#include <memory>
#include <map>
#include <vector>

namespace dakku {

struct RenderOptions {
  std::shared_ptr<Sampler> sampler{nullptr};
  std::shared_ptr<Integrator> integrator{nullptr};
  std::shared_ptr<Scene> scene{nullptr};
  std::shared_ptr<Film> film{nullptr};
  std::shared_ptr<Camera> camera{nullptr};
  std::map<std::string, std::shared_ptr<Material>> materials;
  std::vector<std::shared_ptr<Light>> lights;
  std::vector<std::shared_ptr<Primitive>> primitives;
};

class SceneParser {
 public:
  virtual void parse(const std::string &fileName) = 0;

  RenderOptions renderOptions;
};

class DakkuSceneParser : public SceneParser {
 public:
  void parse(const std::string &fileName) override;
};
}
#endif  // DAKKU_INCLUDE_DAKKU_CORE_PARSER_H_
