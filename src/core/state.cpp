#include <core/state.h>
#include <core/relative.h>
#include <core/fstream.h>
#include <core/class.h>
#include <core/shape.h>
#include <core/primitive.h>
#include <core/material.h>
#include <core/light.h>
#include <core/scene.h>
#include <core/integrator.h>

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

  // lights
  if (json.contains("lights")) {
    const auto &jLight = json["lights"];
    for (auto it = jLight.begin(); it != jLight.end(); ++it) {
      std::string type;
      if (!it->contains("class")) {
        DAKKU_ERR("light type is unknown");
      } else {
        it->at("class").get_to(type);
      }
      std::unique_ptr<Light> pLight{
          dynamic_cast<Light *>(Class::instance().create(type))};
      pLight->unserialize(*it, stream);
      const std::string &lightName = it.key();
      this->lights[lightName] = std::move(pLight);
    }
  }

  std::vector<Light *> allLights;
  std::vector<const Primitive *> allPrimitives;
  for (const auto &[_, p] : this->lights) {
    if (p->getClassName() == "DiffuseAreaLight") {
      auto lightList = dynamic_cast<AreaLight *>(p.get())->getLightList();
      auto primList = dynamic_cast<AreaLight *>(p.get())->getPrimitiveList();
      allLights.insert(allLights.end(), lightList.begin(), lightList.end());
      allPrimitives.insert(allPrimitives.end(), primList.begin(),
                           primList.end());
    } else {
      allLights.push_back(p.get());
    }
  }
  // accelerator
  for (const auto &[_, p] : this->primitives) allPrimitives.push_back(p.get());
  aggregate = std::unique_ptr<Primitive>{
      dynamic_cast<Primitive *>(Class::instance().create("EmbreeAccel"))};
  dynamic_cast<Aggregate *>(aggregate.get())->build(allPrimitives);

  // camera
  if (json.contains("camera")) {
    const auto &jCamera = json["camera"];
    std::string type;
    if (jCamera.contains("class")) {
      jCamera.at("class").get_to(type);
    } else {
      DAKKU_ERR("unknown camera type");
    }
    camera = std::unique_ptr<Camera>{
        dynamic_cast<Camera *>(Class::instance().create(type))};
    camera->unserialize(jCamera, stream);
  }

  // sampler
  if (json.contains("sampler")) {
    const auto &jSampler = json["sampler"];
    std::string type;
    if (jSampler.contains("class")) {
      jSampler.at("class").get_to(type);
    } else {
      DAKKU_ERR("unknown sampler type");
    }
    sampler = std::unique_ptr<Sampler>{
        dynamic_cast<Sampler *>(Class::instance().create(type))};
    sampler->unserialize(jSampler, stream);
  }

  // scene
  scene = std::make_unique<Scene>(aggregate.get(), allLights);

  // integrator
  if (json.contains("integrator")) {
    const auto &jIntegrator = json["integrator"];
    std::string type;
    if (!jIntegrator.contains("class")) {
      DAKKU_ERR("light type is unknown");
    } else {
      jIntegrator.at("class").get_to(type);
    }
    integrator = std::unique_ptr<Integrator>{
        dynamic_cast<Integrator *>(Class::instance().create(type))};
    integrator->unserialize(jIntegrator, stream);
  } else {
    DAKKU_ERR("no integrator");
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
