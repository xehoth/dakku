#include <core/texture.h>

namespace dakku {

Texture::Texture(const Property &p)
    : floatType(p.getNumberIf("floatType", false)) {}
}  // namespace dakku