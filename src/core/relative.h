#ifndef DAKKU_SRC_CORE_RELATIVE_H_
#define DAKKU_SRC_CORE_RELATIVE_H_
#include <core/singleton.h>
#include <string>
#include <filesystem>

DAKKU_BEGIN
/**
 * utility to retrieve relative path (used for scene loading and saving)
 */
struct DAKKU_EXPORT_CORE RelativeRoot final : public Singleton<RelativeRoot> {
 public:
  [[nodiscard]] const std::string &get() const;
  void set(const std::filesystem::path &sceneFilePath);

 private:
  explicit RelativeRoot() = default;
  friend class Singleton<RelativeRoot>;

  std::string dir;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_RELATIVE_H_
