//
// Created by xehoth on 2021/10/1.
//
#include <dakku/core/film.h>
#include <dakku/cameras/perspective.h>
#include <dakku/accelerators/embree_accel.h>
#include <dakku/core/geometry.h>
#include <dakku/core/scene.h>
#include <memory>
#include <dakku/core/interaction.h>
#include <dakku/core/parser.h>
using namespace dakku;


int main(int argc, const char *argv[]) {
  DakkuSceneParser parser;
  parser.parse("../../../scenes/cornell_box.json");

  for (int i = 0; i < 1024; ++i) {
    for (int j = 0; j < 1024; ++j) {
      Ray ray;
      CameraSample sample;
      sample.pFilm = Point2f(i + 0.5, j + 0.5);
      parser.camera->generateRay(sample, ray);
      SurfaceInteraction isect;
      parser.scene->intersect(ray, isect);
      if (isect.primitive != nullptr) {
        Float r = isect.n.x;
        Float g = isect.n.y;
        Float b = isect.n.z;
        r = r * 0.5f + 0.5f;
        g = g * 0.5f + 0.5f;
        b = b * 0.5f + 0.5f;
        parser.film->getPixel(Point2i(i, j)) = RGBSpectrum(r, g, b);
      }
      //      film->getPixel(Point2i(i, j)) = RGBSpectrum(
      //          ray.d.x * 0.5 + 0.5, ray.d.y * 0.5 + 0.5, ray.d.z * 0.5 +
      //          0.5);
      //      //          RGBSpectrum(i / 1920., j / 1080., 0.3);
    }
  }
  parser.film->write("test.png");
  return 0;
}