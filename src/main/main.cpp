#include <core/fwd.h>
#include <logger/logger.h>
#include <math/vector.h>
#include <iostream>

using namespace dakku;

int main(int argc, const char *argv[]) {
  std::cout << "Hello World!" << std::endl;
  Vector3f a(1, 2, 3);
  Point3f b(1, 2, 3);
  
  std::cout << a << std::endl;

  // std::cout << inverse(a) << std::endl;

  // VectorBase<float, 3, Vector<float, 3>> c = b;
  // std::cout << a + b << std::endl;
  // test();
  return 0;
}