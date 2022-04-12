#include <core/state.h>

namespace dakku {

RenderState::RenderState(const Property &) {}

void RenderState::serialize(OutputStream *) const {
  DAKKU_ERR("unimplemented");
}

void RenderState::deserialize(InputStream *) {
  DAKKU_ERR("unimplemented");
}

RenderState renderState;
}  // namespace dakku