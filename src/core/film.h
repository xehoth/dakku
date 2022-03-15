#ifndef DAKKU_SRC_CORE_FILM_H_
#define DAKKU_SRC_CORE_FILM_H_
#include <core/sobject.h>
#include <core/bounds.h>
#include <core/filter.h>

DAKKU_BEGIN
class Film : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Film);

  void serialize(Json &json, OutputStream *stream) const override;
  void unserialize(const Json &json, InputStream *stream) override;

  Point2i fullResolution;
  Float diagonal{0.35f};
  std::unique_ptr<Filter> filter{};
  std::string fileName;
  Bounds2f cropWindow{Point2f(0, 0), Point2f(1, 1)};
  Bounds2i croppedPixelBounds;

 private:
  struct Pixel {
    Float xyz[3];
  };
  std::unique_ptr<Pixel[]> pixels{};
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_FILM_H_
