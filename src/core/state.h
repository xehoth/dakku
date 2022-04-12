#include <core/serialization.h>
#include <core/film.h>

namespace dakku {

class DAKKU_EXPORT_CORE RenderState final : public SerializableObject {
 public:
  DAKKU_DECLARE_OBJECT(RenderState, SerializableObject);

  void serialize(OutputStream *stream) const override;
  void deserialize(InputStream *stream) override;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  std::unique_ptr<Film> film;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};

extern DAKKU_EXPORT_CORE RenderState renderState;
}  // namespace dakku