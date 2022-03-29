#ifndef DAKKU_SRC_CORE_SOBJECT_H_
#define DAKKU_SRC_CORE_SOBJECT_H_
#include <core/object.h>
#include <core/serialization.h>

DAKKU_BEGIN
struct SerializableObject : public Object, public Serializable {
 public:
  DAKKU_DECLARE_OBJECT(SerializableObject, Object);
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_SOBJECT_H_
