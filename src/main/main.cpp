#include <core/fwd.h>
#include <core/logger.h>
#include <core/vector.h>
#include <core/property.h>
#include <core/transform.h>
#include <iostream>

using namespace dakku;

int main(int argc, const char *argv[]) {
  std::cout << "Hello World!" << std::endl;
  Vector3f a(1, 2, 3);
  Point3f b(1, 2, 3);

  Property p;
  p[0] = a;
  Matrix4x4 m;
  std::cout << m[0][0] << std::endl;

  // std::cout << inverse(a) << std::endl;

  // VectorBase<float, 3, Vector<float, 3>> c = b;
  // std::cout << a + b << std::endl;
  // test();
  return 0;
}