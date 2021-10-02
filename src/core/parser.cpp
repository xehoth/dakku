//
// Created by xehoth on 2021/10/2.
//
#include <dakku/core/parser.h>
#include <dakku/core/transform.h>
#include <dakku/cameras/perspective.h>
#include <dakku/core/film.h>
#include <dakku/shapes/triangle.h>
#include <dakku/core/primitive.h>
#include <dakku/accelerators/embree_accel.h>
#include <dakku/core/light.h>
#include <dakku/core/scene.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
namespace dakku {

using Json = nlohmann::json;

decltype(auto) createPrimsFromMesh(
    std::vector<std::shared_ptr<Shape>> &shapes) {
  std::shared_ptr<std::vector<std::shared_ptr<Primitive>>> prims =
      std::make_shared<std::vector<std::shared_ptr<Primitive>>>();
  for (auto &p : shapes) {
    prims->push_back(std::make_shared<GeometricPrimitive>(p));
  }
  return prims;
}

void DakkuSceneParser::parse(const std::string &fileName) {
  std::ifstream in(fileName);
  Json json;
  in >> json;

  auto parseCamera = [&]() {
    auto camera = json["camera"];
    auto parseFilm = [&]() {
      Point2i resolution;
      resolution.x = camera["resolution"][0];
      resolution.y = camera["resolution"][1];
      return std::make_shared<Film>(resolution);
    };

    auto film = parseFilm();
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
    return std::make_shared<PerspectiveCamera>(cameraToWorld, fov, film);
  };

  auto parsePrimitives = [&]() {
    auto prims =
        std::make_shared<std::vector<EmbreeAccel::TrianglePrimitivePair>>();
    auto primitives = json["primitives"];
    for (const auto &primitive : primitives) {
      auto parseRawMeshShape = [&]() {
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

      std::string type = primitive["type"];
      if (type == "raw_mesh") {
        auto shapes = parseRawMeshShape();
        auto triPrim = createPrimsFromMesh(*shapes.second);
        prims->emplace_back(shapes.first, triPrim);
      }
    }
    return std::make_shared<EmbreeAccel>(prims);
  };

  std::vector<std::shared_ptr<Light>> lights;
  this->camera = parseCamera();
  this->film = camera->film;
  std::shared_ptr<Primitive> aggregate = parsePrimitives();
  this->scene = std::make_shared<Scene>(aggregate, lights);
}

}  // namespace dakku