#include <core/fwd.h>
#include <logger/logger.h>
#include <math/vector_base.h>
#include <iostream>

using namespace dakku;

int main(int argc, const char *argv[]) {
  std::cout << "Hello World!" << std::endl;
  VectorBase<float, 2, float> a;
  a.setByIndex(2, 2);
  
  test();
  return 0;
}