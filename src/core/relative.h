#ifndef DAKKU_CORE_RELATIVE_H_
#define DAKKU_CORE_RELATIVE_H_
#include <core/fwd.h>
#include <string>
#include <filesystem>

namespace dakku {
/**
 * @brief utility to retrieve relative path (used for scene loading and saving)
 */
class DAKKU_EXPORT_CORE RelativeRoot final {
 public:
  /**
   * @brief get the relative root instance
   *
   * @return RelativeRoot&
   */
  static RelativeRoot &instance();

  /**
   * @brief get the path
   *
   */
  [[nodiscard]] const std::string &get() const;

  /**
   * @brief add relative path to the given path
   *
   */
  [[nodiscard]] std::filesystem::path get(
      const std::filesystem::path &path) const;

  /**
   * @brief add relative path to the given path
   *
   */
  [[nodiscard]] std::string get(const std::string &path) const;

  /**
   * @brief set the relative root
   *
   */
  void set(const std::filesystem::path &root);

 private:
  explicit RelativeRoot() = default;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

  std::string dir;

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};
}  // namespace dakku
#endif