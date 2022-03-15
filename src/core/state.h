#ifndef DAKKU_SRC_CORE_STATE_H_
#define DAKKU_SRC_CORE_STATE_H_
#include <core/sobject.h>
#include <core/film.h>
#include <filesystem>

DAKKU_BEGIN
class RenderState final : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(RenderState);

  static std::unique_ptr<RenderState> load(const std::filesystem::path &path);
  void save(const std::filesystem::path &path);

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

  Film film;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_STATE_H_
