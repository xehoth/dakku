#ifndef DAKKU_CORE_SERIALIZATION_H_
#define DAKKU_CORE_SERIALIZATION_H_
#include <core/object.h>

namespace dakku {

/**
 * @brief input stream
 */
class DAKKU_EXPORT_CORE InputStream : public Object {
 public:
  DAKKU_DECLARE_OBJECT(InputStream, Object);

  /**
   * @brief read next byte
   * @return the next byte
   */
  virtual std::uint8_t readByte() = 0;
  /**
   * @brief read bytes
   * @param ptr the output address
   * @param size the size of bytes
   * @return the size actually read
   */
  virtual size_t readBytes(void *ptr, size_t size) = 0;
};

/**
 * @brief output stream
 */
class DAKKU_EXPORT_CORE OutputStream : public Object {
 public:
  DAKKU_DECLARE_OBJECT(OutputStream, Object);
  /**
   * @brief write a byte
   * @param value the value of the byte
   */
  virtual void writeByte(std::uint8_t value) = 0;
  /**
   * @brief write bytes
   * @param ptr the start address of the bytes to write
   * @param size the size of bytes
   * @return the size actually write
   */
  virtual size_t writeBytes(const void *ptr, size_t size) = 0;
};

/**
 * @brief serializable object
 *
 */
class DAKKU_EXPORT_CORE SerializableObject : public Object {
 public:
  DAKKU_DECLARE_OBJECT(SerializableObject, Object);

  /**
   * @brief serialize the object (do not include the property part)
   *
   * @param stream
   */
  virtual void serialize(OutputStream *stream) const = 0;

  /**
   * @brief deserialize the object (do not include the property part)
   *
   * @param stream
   */
  virtual void deserialize(InputStream *stream) const = 0;
};
}  // namespace dakku
#endif