#include <core/state.h>
#include <core/relative.h>
#include <core/fstream.h>
#include <core/class.h>
#include <core/shape.h>
#include <core/primitive.h>
#include <core/material.h>

DAKKU_BEGIN

void RenderState::serialize(Json &json, OutputStream *stream) const {
  film.serialize(json["film"], stream);
  // shapes
  auto &jShape = json["shapes"];
  for (auto &[k, v] : shapes) v->serialize(jShape[k], stream);
  // materials
//  auto &jMaterial = json["materials"];
//  for (auto &[k, v] : materials) v->serialize(jMaterial[k], stream);
  // primitives
//  auto &jPrimitive = json["primitives"];
//  for (auto &[k, v] : primitives) v->serialize(jPrimitive[k], stream);
}

void RenderState::unserialize(const Json &json, InputStream *stream) {
  film.unserialize(json["film"], stream);
  // shapes
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

  // materials

  // primitives
//  if (json.contains("primitives")) {
//    const auto &jPrimitive = json["primitives"];
//    for (auto it = jPrimitive.begin(); it != jPrimitive.end(); ++it) {
//      std::string type;
//      if (!it->contains("class")) {
//        DAKKU_ERR("primitive type is unknown");
//      } else {
//        it->at("class").get_to(type);
//      }
//      std::string shape, material;
//      const Shape *pShape{};
//      const Material *pMaterial{};
//      if (!it->contains("shape")) {
//        DAKKU_ERR("primitive does not have a shape");
//      } else {
//        it->at("shape").get_to(shape);
//        if (auto shapeIt = this->shapes.find(shape);
//            shapeIt != this->shapes.end()) {
//          pShape = shapeIt->second.get();
//        } else {
//          DAKKU_WARN("cannot find shape {} in shape map, use nullptr", shape);
//        }
//      }
//      if (!it->contains("material")) {
//        DAKKU_ERR("primitive does not have a material");
//      } else {
//        it->at("material").get_to(material);
//        if (auto matIt = this->materials.find(material);
//            matIt != this->materials.end()) {
//          pMaterial = matIt->second.get();
//        } else {
//          DAKKU_ERR("cannot find material {} in material map, use nullptr",
//                    material);
//        }
//      }
//      std::unique_ptr<Primitive> pPrimitive{
//          dynamic_cast<Primitive *>(Class::instance().create(type))};
//      pPrimitive->unserialize(*it, stream);
//      this->primitives[it.key()] = std::move(pPrimitive);
//    }
//  }
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
