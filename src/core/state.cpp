#include <core/state.h>
#include <core/relative.h>
#include <core/fstream.h>
#include <core/class.h>
#include <core/shape.h>

DAKKU_BEGIN

void RenderState::serialize(Json &json, OutputStream *stream) const {
  film.serialize(json["film"], stream);
  auto &jShape = json["shapes"];
  for (auto &[k, v] : shapes) v->serialize(jShape[k], stream);
}

void RenderState::unserialize(const Json &json, InputStream *stream) {
  film.unserialize(json["film"], stream);
  if (json.contains("shapes")) {
    const auto &jShapes = json["shapes"];
    for (auto it = jShapes.begin(); it != jShapes.end(); ++it) {
      std::string shapeType;
      if (!it->contains("class")) {
        DAKKU_ERR("shape type is unknown");
      } else {
        it->at("class").get_to(shapeType);
      }
      std::unique_ptr<Shape> pShape{
          dynamic_cast<Shape *>(Class::instance().create(shapeType))};
      pShape->unserialize(*it, stream);
      this->shapes[it.key()] = std::move(pShape);
    }
  } else {
    DAKKU_WARN("no shapes in the scene");
  }
}

void RenderState::load(const std::filesystem::path &path) {
  RelativeRoot::instance().set(path);

  Json json;
  std::ifstream jStream(path);
  jStream >> json;
  std::unique_ptr<FileInputStream> stream{};
  if (json.contains("serialized")) {
    std::string serFile;
    json.at("serialized").get_to(serFile);
    if (std::filesystem::exists(RelativeRoot::instance().get() + serFile)) {
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
  RelativeRoot::instance().set(path);
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
