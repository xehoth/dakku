#ifndef DAKKU_SRC_CORE_STATE_H_
#define DAKKU_SRC_CORE_STATE_H_
#include <core/sobject.h>
#include <core/film.h>
#include <filesystem>
#include <map>

DAKKU_BEGIN
class RenderState final : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(RenderState);

  void load(const std::filesystem::path &path);
  void save(const std::filesystem::path &path);

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

  Film film;
  std::map<std::string, std::unique_ptr<Shape>> shapes;
};

extern DAKKU_EXPORT_CORE RenderState renderState;
DAKKU_END
#endif  // DAKKU_SRC_CORE_STATE_H_
