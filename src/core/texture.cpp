#include <core/texture.h>
#include <core/interaction.h>
namespace dakku {

UvMapping2D::UvMapping2D(float su, float sv, float du, float dv)
    : su(su), sv(sv), du(du), dv(dv) {}
UvMapping2D::UvMapping2D(const Property &p)
    : UvMapping2D(p.getNumberIf("su", 1.0f), p.getNumberIf("sv", 1.0f),
                  p.getNumberIf("du", 0.0f), p.getNumberIf("dv", 0.0f)) {}

Point2f UvMapping2D::map(const SurfaceInteraction &si, Vector2f &dstdx,
                         Vector2f &dstdy) const {
  // compute texture differentials for 2D identity mapping
  dstdx = Vector2f(su * si.dudx, sv * si.dvdx);
  dstdy = Vector2f(su * si.dudy, sv * si.dvdy);
  return Point2f{su * si.uv[0] + du, sv * si.uv[1] + dv};
}

Texture::Texture(bool floatType) : floatType(floatType) {}

}  // namespace dakku