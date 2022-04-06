#include <core/fwd.h>
#include <logger/logger.h>
#include <math/vector_base.h>
#include <iostream>

using namespace dakku;

int main(int argc, const char *argv[]) {
  std::cout << "Hello World!" << std::endl;
  VectorBase<float, 2, float> a(1, 2);
  VectorBase<float, 2, float> b(3, 4);
  const auto &c = a;
  c[2];
  std::cout << a << " " << b << std::endl;
  a.set(b);
  VectorBase<float, 2, float> d(a);
  std::cout << d << std::endl;
  a = a + b;
  std::cout << a << std::endl;
  // test();
  return 0;
}