//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/parser.h>
#include <dakku/core/transform.h>
#include <dakku/cameras/perspective.h>
#include <dakku/core/film.h>
#include <dakku/core/primitive.h>
#include <dakku/core/light.h>
#include <dakku/core/scene.h>
#include <dakku/core/material.h>
#include <dakku/shapes/triangle.h>
#include <dakku/accelerators/embree_accel.h>
#include <dakku/materials/matte.h>
#include <dakku/lights/diffuse.h>
#include <dakku/samplers/random.h>
#include <dakku/integrators/path.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
namespace dakku {

using Json = nlohmann::json;
void parseMaterials(const Json &json, RenderOptions &renderOptions);
void parseCamera(const Json &json, RenderOptions &renderOptions);
void parsePrimitives(const Json &json, RenderOptions &renderOptions);
void parseSampler(const Json &json, RenderOptions &renderOptions);
void parseIntegrator(const Json &json, RenderOptions &renderOptions);

void DakkuSceneParser::parse(const std::string &fileName) {
  std::ifstream in(fileName);
  Json json;
  in >> json;

  parseMaterials(json, this->renderOptions);
  parseCamera(json, this->renderOptions);
  parsePrimitives(json, this->renderOptions);
  parseSampler(json, this->renderOptions);
  parseIntegrator(json, this->renderOptions);
  std::clog << "Lights: " << renderOptions.lights.size() << std::endl;
  std::clog << "spp: " << renderOptions.sampler->samplesPerPixel << std::endl;

  //  auto parseMaterials = [&]() {
  //    std::map<std::string, std::shared_ptr<Material>> materials;
  //    for (auto &mat : json["materials"]) {
  //      std::string name = mat["name"];
  //      std::string type = mat["type"];
  //      if (type == "matte") {
  //        RGBSpectrum Kd;
  //        Kd.x = mat["Kd"][0];
  //        Kd.y = mat["Kd"][1];
  //        Kd.z = mat["Kd"][2];
  //        materials[name] = std::make_shared<MatteMaterial>(Kd);
  //      }
  //    }
  //    return materials;
  //  };

  //  auto materials = parseMaterials();
  //
  //  auto parseCamera = [&]() {
  //    auto camera = json["camera"];
  //    auto parseFilm = [&]() {
  //      Point2i resolution;
  //      resolution.x = camera["resolution"][0];
  //      resolution.y = camera["resolution"][1];
  //      return std::make_shared<Film>(resolution);
  //    };
  //
  //    auto film = parseFilm();
  //    Point3f pos;
  //    Vector3f look;
  //    Vector3f up;
  //    for (int i = 0; i < 3; ++i) {
  //      pos[i] = camera["transform"]["position"][i];
  //      look[i] = camera["transform"]["look"][i];
  //      up[i] = camera["transform"]["up"][i];
  //    }
  //    Transform worldToCamera = lookAt(pos, look, up);
  //    Transform cameraToWorld = inverse(worldToCamera);
  //    Float fov = camera["fov"];
  //    return std::make_shared<PerspectiveCamera>(cameraToWorld, fov, film);
  //  };

  //  std::vector<std::shared_ptr<Light>> lights;
  //
  //  auto parsePrimitives = [&]() {
  //    auto prims =
  //        std::make_shared<std::vector<EmbreeAccel::TrianglePrimitivePair>>();
  //    auto primitives = json["primitives"];
  //
  //    auto parseRawMeshShape = [&](auto &primitive) {
  //      auto data = primitive["data"];
  //      auto index = data["index"];
  //      auto pos = data["position"];
  //      auto normal = data["normal"];
  //      auto uv = data["uv"];
  //
  //      std::vector<int> indices = index;
  //      int nVertices = static_cast<int>(pos.size() / 3);
  //      std::vector<Point3f> p;
  //      std::vector<Normal3f> n;
  //      std::vector<Point2f> tex;
  //      for (int i = 0; i < nVertices; ++i) {
  //        Point3f tp;
  //        Normal3f tn;
  //        Point2f tuv;
  //        for (int j = 0; j < 3; ++j) {
  //          tp[j] = pos[i * 3 + j];
  //          tn[j] = normal[i * 3 + j];
  //        }
  //        tuv.s = uv[i * 2 + 0];
  //        tuv.t = uv[i * 2 + 1];
  //        p.push_back(tp);
  //        n.push_back(tn);
  //        tex.push_back(tuv);
  //      }
  //      // TODO: add transform
  //      Transform o2w;
  //      return createTriangleMesh(o2w, indices, nVertices, p, n, tex);
  //    };
  //
  //    for (const auto &primitive : primitives) {
  //      std::string type = primitive["type"];
  //      std::string matName = primitive["material"];
  //      if (type == "raw_mesh") {
  //        auto shapes = parseRawMeshShape(primitive);
  //        auto triPrim = createPrimsFromMesh(*shapes.second,
  //        materials[matName]); prims->emplace_back(shapes.first, triPrim);
  //      }
  //    }
  //
  //    // parse light primitives
  //    for (auto &light : json["lights"]) {
  //      RGBSpectrum power;
  //      power.x = light["power"][0];
  //      power.y = light["power"][1];
  //      power.z = light["power"][2];
  //
  //      std::string type = light["type"];
  //      std::string matName = light["material"];
  //      std::string shapeType = light["shape"]["type"];
  //      if (shapeType == "raw_mesh") {
  //        auto shapes = parseRawMeshShape(light["shape"]);
  //        std::shared_ptr<std::vector<std::shared_ptr<Primitive>>> triPrim =
  //            std::make_shared<std::vector<std::shared_ptr<Primitive>>>();
  //        for (auto &sh : *shapes.second) {
  //          std::shared_ptr<AreaLight> l{nullptr};
  //          if (type == "diffuse") {
  //            l = std::make_shared<DiffuseAreaLight>(Transform{}, power, sh);
  //          }
  //          lights.push_back(l);
  //          triPrim->push_back(
  //              std::make_shared<GeometricPrimitive>(sh, materials[matName],
  //              l));
  //        }
  //        prims->emplace_back(shapes.first, triPrim);
  //      }
  //    }
  //
  //    return std::make_shared<EmbreeAccel>(prims);
  //  };

  //  this->camera = parseCamera();
  //  this->film = camera->film;
  //  std::shared_ptr<Primitive> aggregate = parsePrimitives();
  //  this->scene = std::make_shared<Scene>(aggregate, lights);
}

decltype(auto) createPrimsFromMesh(std::vector<std::shared_ptr<Shape>> &shapes,
                                   std::shared_ptr<Material> mat) {
  std::shared_ptr<std::vector<std::shared_ptr<Primitive>>> prims =
      std::make_shared<std::vector<std::shared_ptr<Primitive>>>();
  for (auto &p : shapes) {
    prims->push_back(std::make_shared<GeometricPrimitive>(p, mat));
  }
  return prims;
}

void parseMaterials(const Json &json, RenderOptions &renderOptions) {
  for (auto &mat : json["materials"]) {
    std::string name = mat["name"];
    std::string type = mat["type"];
    if (type == "matte") {
      RGBSpectrum Kd;
      Kd.x = mat["Kd"][0];
      Kd.y = mat["Kd"][1];
      Kd.z = mat["Kd"][2];
      renderOptions.materials[name] = std::make_shared<MatteMaterial>(Kd);
    }
  }
}

void parseCamera(const Json &json, RenderOptions &renderOptions) {
  auto camera = json["camera"];
  Point2i resolution;
  resolution.x = camera["resolution"][0];
  resolution.y = camera["resolution"][1];
  auto film = std::make_shared<Film>(resolution);
  renderOptions.film = film;

  Point3f pos;
  Vector3f look;
  Vector3f up;
  for (int i = 0; i < 3; ++i) {
    pos[i] = camera["transform"]["position"][i];
    look[i] = camera["transform"]["look"][i];
    up[i] = camera["transform"]["up"][i];
  }
  Transform worldToCamera = lookAt(pos, look, up);
  Transform cameraToWorld = inverse(worldToCamera);
  Float fov = camera["fov"];

  renderOptions.camera =
      std::make_shared<PerspectiveCamera>(cameraToWorld, fov, film);
}

void parsePrimitives(const Json &json, RenderOptions &renderOptions) {
  auto prims =
      std::make_shared<std::vector<EmbreeAccel::TrianglePrimitivePair>>();
  auto primitives = json["primitives"];
  auto parseRawMeshShape = [&](auto &primitive) {
    auto data = primitive["data"];
    auto index = data["index"];
    auto pos = data["position"];
    auto normal = data["normal"];
    auto uv = data["uv"];

    std::vector<int> indices = index;
    int nVertices = static_cast<int>(pos.size() / 3);
    std::vector<Point3f> p;
    std::vector<Normal3f> n;
    std::vector<Point2f> tex;
    for (int i = 0; i < nVertices; ++i) {
      Point3f tp;
      Normal3f tn;
      Point2f tuv;
      for (int j = 0; j < 3; ++j) {
        tp[j] = pos[i * 3 + j];
        tn[j] = normal[i * 3 + j];
      }
      tuv.s = uv[i * 2 + 0];
      tuv.t = uv[i * 2 + 1];
      p.push_back(tp);
      n.push_back(tn);
      tex.push_back(tuv);
    }
    // TODO: add transform
    Transform o2w;
    return createTriangleMesh(o2w, indices, nVertices, p, n, tex);
  };

  for (const auto &primitive : primitives) {
    std::string type = primitive["type"];
    std::string matName = primitive["material"];
    if (type == "raw_mesh") {
      auto shapes = parseRawMeshShape(primitive);
      auto triPrim =
          createPrimsFromMesh(*shapes.second, renderOptions.materials[matName]);
      prims->emplace_back(shapes.first, triPrim);
    }
  }

  // parse light primitives
  for (auto &light : json["lights"]) {
    RGBSpectrum power;
    power.x = light["power"][0];
    power.y = light["power"][1];
    power.z = light["power"][2];

    std::string type = light["type"];
    std::string matName = light["material"];
    std::string shapeType = light["shape"]["type"];
    if (shapeType == "raw_mesh") {
      auto shapes = parseRawMeshShape(light["shape"]);
      std::shared_ptr<std::vector<std::shared_ptr<Primitive>>> triPrim =
          std::make_shared<std::vector<std::shared_ptr<Primitive>>>();
      for (auto &sh : *shapes.second) {
        std::shared_ptr<AreaLight> l{nullptr};
        if (type == "diffuse") {
          l = std::make_shared<DiffuseAreaLight>(Transform{}, power, sh);
        }
        renderOptions.lights.push_back(l);
        triPrim->push_back(std::make_shared<GeometricPrimitive>(
            sh, renderOptions.materials[matName], l));
      }
      prims->emplace_back(shapes.first, triPrim);
    }
  }
  std::shared_ptr<Primitive> aggregate = std::make_shared<EmbreeAccel>(prims);
  renderOptions.scene =
      std::make_shared<Scene>(aggregate, renderOptions.lights);
}

void parseSampler(const Json &json, RenderOptions &renderOptions) {
  auto jSampler = json["sampler"];
  std::string type = jSampler["type"];
  std::uint32_t spp = jSampler["spp"];
  std::shared_ptr<Sampler> sampler{nullptr};
  if (type == "random") {
    sampler = std::make_shared<RandomSampler>(spp);
  }
  renderOptions.sampler = sampler;
}

void parseIntegrator(const Json &json, RenderOptions &renderOptions) {
  auto jIntegrator = json["integrator"];
  std::string type = jIntegrator["type"];
  std::shared_ptr<Integrator> integrator{nullptr};
  if (type == "path") {
    auto jData = jIntegrator["data"];
    int maxDepth = jData["depth"];
    integrator = std::make_shared<PathIntegrator>(
        maxDepth, renderOptions.camera, renderOptions.sampler);
  }
  renderOptions.integrator = integrator;
}
}  // namespace dakku
