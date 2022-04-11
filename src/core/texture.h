#ifndef DAKKU_CORE_TEXTURE_H_
#define DAKKU_CORE_TEXTURE_H_
#include <core/object.h>

namespace dakku {

/**
 * @brief 2d texture mapping
 *
 */
class TextureMapping2D : public Object {
 public:
  DAKKU_DECLARE_OBJECT(TextureMapping2D, Object);

  virtual Point2f map(const SurfaceInteraction &si, Vector2f &dstdx,
                      Vector2f &dstdy) const = 0;
};

class UvMapping2D : public TextureMapping2D {
 public:
  DAKKU_DECLARE_OBJECT(UvMapping2D, TextureMapping2D);

  explicit UvMapping2D(float su = 1, float sv = 1, float du = 0, float dv = 0);

  Point2f map(const SurfaceInteraction &si, Vector2f &dstdx,
              Vector2f &dstdy) const override;

 private:
  const float su, sv;
  const float du, dv;
};

class DAKKU_EXPORT_CORE Texture : public Object {
 public:
  DAKKU_DECLARE_OBJECT(Texture, Object);

  explicit Texture(bool floatType);

  [[nodiscard]] virtual Spectrum evaluate(const SurfaceInteraction &) const = 0;

 protected:
  bool floatType{};
};
}  // namespace dakku
#endif