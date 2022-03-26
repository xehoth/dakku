#ifndef DAKKU_SRC_CORE_SERIALIZATION_H_
#define DAKKU_SRC_CORE_SERIALIZATION_H_
#include <core/fwd.h>
#include <nlohmann/json.hpp>

DAKKU_BEGIN

using Json = nlohmann::json;

struct Serializable {
  virtual void serialize(Json &json, OutputStream *stream) const = 0;
  virtual void unserialize(const Json &json, InputStream *stream) = 0;
};

/**
 * serialize name to json
 */
#define DAKKU_SER_J(name) json[#name] = name
/**
 * unserialize from json to name
 */
#define DAKKU_UNSER_J(name) json.at(#name).get_to(name)
/**
 * if name exists then unserialize
 */
#define DAKKU_UNSER_JI(name) \
  if (json.contains(#name) && !json.at(#name).is_null()) DAKKU_UNSER_J(name)
/**
 * if name exists then unserialize
 * else set to value
 */
#define DAKKU_UNSER_JIE(name, value) \
  if (json.contains(#name))          \
    DAKKU_UNSER_J(name);             \
  else                               \
    name = value

DAKKU_END
#endif  // DAKKU_SRC_CORE_SERIALIZATION_H_
