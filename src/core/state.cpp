#include <core/state.h>

namespace dakku {

void RenderState::serialize(OutputStream *) const {
  DAKKU_ERR("unimplemented");
}

void RenderState::deserialize(InputStream *) {
  DAKKU_ERR("unimplemented");
}

RenderState renderState;
}  // namespace dakku