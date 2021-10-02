//
// Created by xehoth on 2021/10/2.
//

#ifndef DAKKU_INCLUDE_DAKKU_CORE_PARSER_H_
#define DAKKU_INCLUDE_DAKKU_CORE_PARSER_H_
#include <dakku/core/dakku.h>
#include <string>
#include <memory>

namespace dakku {

class SceneParser {
 public:
  virtual void parse(const std::string &fileName) = 0;

  std::shared_ptr<Scene> scene{nullptr};
  std::shared_ptr<Film> film{nullptr};
  std::shared_ptr<Camera> camera{nullptr};
};

class DakkuSceneParser : public SceneParser {
 public:
  void parse(const std::string &fileName) override;
};
}
#endif  // DAKKU_INCLUDE_DAKKU_CORE_PARSER_H_
