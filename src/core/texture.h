#ifndef DAKKU_CORE_TEXTURE_H_
#define DAKKU_CORE_TEXTURE_H_
#include <core/object.h>

namespace dakku {

class DAKKU_EXPORT_CORE Texture : public Object {
 public:
  DAKKU_DECLARE_OBJECT(Texture, Object);
  
  [[nodiscard]] virtual Spectrum evaluate() const = 0;

 protected:
  bool floatType{};
};
}  // namespace dakku
#endif