#include <core/relative.h>

namespace dakku {

RelativeRoot &RelativeRoot::instance() {
  static RelativeRoot _instance;
  return _instance;
}

const std::string &RelativeRoot::get() const { return dir; }

void RelativeRoot::set(const std::filesystem::path &root) {
  std::string path = root.parent_path().string();
  if (!path.ends_with('/')) path += "/";
  dir = path;
}

}  // namespace dakku