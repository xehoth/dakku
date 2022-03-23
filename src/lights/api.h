#ifndef DAKKU_SRC_LIGHTS_API_H_
#define DAKKU_SRC_LIGHTS_API_H_
#include <core/class.h>
#include <lights/diffuse.h>

DAKKU_BEGIN
namespace lights {
void init() { DAKKU_REGISTER_CLASS(DiffuseAreaLight); }
}  // namespace lights
DAKKU_END
#endif  // DAKKU_SRC_LIGHTS_API_H_
