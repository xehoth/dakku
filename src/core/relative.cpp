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

std::filesystem::path RelativeRoot::get(
    const std::filesystem::path &path) const {
  return std::filesystem::path{dir + path.string()};
}

std::string RelativeRoot::get(const std::string &path) const {
  return dir + path;
}
}  // namespace dakku