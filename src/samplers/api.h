#ifndef DAKKU_SRC_SAMPLERS_API_H_
#define DAKKU_SRC_SAMPLERS_API_H_
#include <core/class.h>
#include <samplers/random.h>

DAKKU_BEGIN
namespace samplers {
void init() { DAKKU_REGISTER_CLASS(RandomSampler); }
}  // namespace samplers
DAKKU_END
#endif  // DAKKU_SRC_SAMPLERS_API_H_
