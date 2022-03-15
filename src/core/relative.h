#ifndef DAKKU_SRC_CORE_RELATIVE_H_
#define DAKKU_SRC_CORE_RELATIVE_H_
#include <core/fwd.h>
#include <string>
#include <filesystem>

DAKKU_BEGIN
struct RelativeRoot final {
 public:
  static const std::string &get() { return dir(); }
  static void set(const std::filesystem::path &sceneFilePath) {
    std::string path = sceneFilePath.parent_path().string();
    if (!path.ends_with('/')) path += "/";
    dir() = path;
  }

 private:
  static std::string &dir() {
    static std::string str;
    return str;
  }
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_RELATIVE_H_
