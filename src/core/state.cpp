#include <core/state.h>
#include <core/relative.h>
#include <core/fstream.h>

DAKKU_BEGIN

void RenderState::serialize(Json &json, OutputStream *stream) const {
  film.serialize(json["film"], stream);
}
void RenderState::unserialize(const Json &json, InputStream *stream) {
  film.unserialize(json["film"], stream);
}

void RenderState::load(const std::filesystem::path &path) {
  RelativeRoot::set(path);

  Json json;
  std::ifstream jStream(path);
  jStream >> json;
  std::unique_ptr<FileInputStream> stream{};
  if (json.contains("serialized")) {
    std::string serFile;
    json.at("serialized").get_to(serFile);
    if (std::filesystem::exists(RelativeRoot::get() + serFile)) {
      stream = std::make_unique<FileInputStream>(serFile);
    } else {
      DAKKU_WARN("cannot find serialized data file: {}", serFile);
    }
  } else {
    DAKKU_INFO("load render state without serialized data");
  }
  this->unserialize(json, stream.get());
}

void RenderState::save(const std::filesystem::path &path) {
  RelativeRoot::set(path);
  Json json;
  std::string serFile = path.stem().string() + "_serialized.dat";
  json["serialized"] = serFile;

  FileOutputStream stream(serFile);
  serialize(json, &stream);

  std::ofstream jStream(path);
  jStream << json;
}

RenderState renderState;
DAKKU_END
