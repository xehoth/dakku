#include <core/fwd.h>
#include <core/logger.h>
#include <core/vector.h>
#include <core/property.h>
#include <core/transform.h>
#include <core/class.h>
#include <core/object.h>
#include <core/bounds.h>
#include <iostream>

using namespace dakku;

int main(int argc, const char *argv[]) {
  std::cout << "Hello World!" << std::endl;
  Vector3f a(1, 2, 3);
  Point3f b(1, 2, 3);

  Property p;
  p[0] = a;
  Matrix4x4 m;
  std::cout << p << std::endl;
  Object *obj = Class::instance().create("Object");
  std::cout << obj->getClassName();
  delete obj;
  printf("%p\n", &Class::instance());
  // std::cout << inverse(a) << std::endl;

  // VectorBase<float, 3, Vector<float, 3>> c = b;
  // std::cout << a + b << std::endl;
  // test();
  return 0;
}