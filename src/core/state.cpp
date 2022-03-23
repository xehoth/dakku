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
  // textures
  auto &jTexture = json["textures"];
  for (auto &[k, v] : floatTextures) {
    jTexture[k]["float"] = true;
    v->serialize(jTexture[k], stream);
  }
  for (auto &[k, v] : spectrumTextures) {
    jTexture[k]["float"] = false;
    v->serialize(jTexture[k], stream);
  }

  // materials
  auto &jMaterial = json["materials"];
  for (auto &[k, v] : materials) v->serialize(jMaterial[k], stream);
  // primitives
  auto &jPrimitive = json["primitives"];
  for (auto &[k, v] : primitives) v->serialize(jPrimitive[k], stream);
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
  // textures
  if (json.contains("textures")) {
    const auto &jTexture = json["textures"];
    for (auto it = jTexture.begin(); it != jTexture.end(); ++it) {
      std::string type;
      if (!it->contains("class")) {
        DAKKU_ERR("texture type is unknown");
      } else {
        it->at("class").get_to(type);
      }
      bool isFloatTexture = false;
      if (!it->contains("float")) {
        DAKKU_WARN("float / spectrum texture is unknown, default use spectrum");
      } else {
        it->at("float").get_to(isFloatTexture);
      }
      if (isFloatTexture) {
        std::unique_ptr<Texture<Float>> pTex{
            dynamic_cast<Texture<Float> *>(Class::instance().create(type))};
        pTex->unserialize(*it, stream);
        this->floatTextures[it.key()] = std::move(pTex);
      } else {
        std::unique_ptr<Texture<Spectrum>> pTex{
            dynamic_cast<Texture<Spectrum> *>(Class::instance().create(type))};
        pTex->unserialize(*it, stream);
        this->spectrumTextures[it.key()] = std::move(pTex);
      }
    }
  } else {
    DAKKU_WARN("no textures in the scene");
  }

  // materials
  if (json.contains("materials")) {
    const auto &jMaterial = json["materials"];
    for (auto it = jMaterial.begin(); it != jMaterial.end(); ++it) {
      std::string type;
      if (!it->contains("class")) {
        DAKKU_ERR("material type is unknown");
      } else {
        it->at("class").get_to(type);
      }
      std::unique_ptr<Material> pMat{
          dynamic_cast<Material *>(Class::instance().create(type))};
      pMat->unserialize(*it, stream);
      this->materials[it.key()] = std::move(pMat);
    }
  }

  // primitives
  if (json.contains("primitives")) {
    const auto &jPrimitive = json["primitives"];
    for (auto it = jPrimitive.begin(); it != jPrimitive.end(); ++it) {
      std::string type;
      if (!it->contains("class")) {
        DAKKU_ERR("primitive type is unknown");
      } else {
        it->at("class").get_to(type);
      }
      std::unique_ptr<Primitive> pPrimitive{
          dynamic_cast<Primitive *>(Class::instance().create(type))};
      pPrimitive->unserialize(*it, stream);
      this->primitives[it.key()] = std::move(pPrimitive);
    }
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
