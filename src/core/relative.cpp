#include <core/relative.h>

DAKKU_BEGIN

const std::string &RelativeRoot::get() const { return dir; }

void RelativeRoot::set(const std::filesystem::path &sceneFilePath) {
  std::string path = sceneFilePath.parent_path().string();
  if (!path.ends_with('/')) path += "/";
  dir = path;
}

DAKKU_END