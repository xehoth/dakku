#ifndef DAKKU_SRC_INTEGRATORS_API_H_
#define DAKKU_SRC_INTEGRATORS_API_H_
#include <core/class.h>
#include <integrators/path.h>

DAKKU_BEGIN
namespace integrators {
void init() { DAKKU_REGISTER_CLASS(PathIntegrator); }
}  // namespace integrators
DAKKU_END
#endif  // DAKKU_SRC_INTEGRATORS_API_H_
