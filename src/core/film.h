#ifndef DAKKU_CORE_FILM_H_
#define DAKKU_CORE_FILM_H_
#include <core/serialization.h>
#include <core/bounds.h>

namespace dakku {
/**
 * @brief film class
 *
 */
class DAKKU_EXPORT_CORE Film : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(Film, SerializableObject);

  // explicit Film(const Point2i &fullResolution, const Bounds2f cropWindow, )

  void serialize(OutputStream *stream) const override;
  void deserialize(InputStream *stream) override;

 private:
  /**
   * @brief pixel struct
   *
   */
  struct Pixel {
    /// color data (in xyz)
    float xyz[3];
    /// sum of the filter weight
    float filterWeightSum;
  };

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  /// pixels data
  std::unique_ptr<Pixel[]> pixels{};
  /// full resolution of the film
  Point2i fullResolution;
  /// film crop window
  Bounds2f cropWindow{Point2f(0, 0), Point2f(1, 1)};
  Bounds2i croppedPixelBounds;

  /// filter table width
  static constexpr int filterTableWidth = 16;

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};
}  // namespace dakku
#endif
