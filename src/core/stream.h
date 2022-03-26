#ifndef DAKKU_SRC_CORE_STREAM_H_
#define DAKKU_SRC_CORE_STREAM_H_
#include <core/fwd.h>
DAKKU_BEGIN
/**
 * dakku input stream
 */
class InputStream {
 public:
  /**
   * read next byte
   * @return the next byte
   */
  virtual std::uint8_t readByte() = 0;
  /**
   * read bytes
   * @param ptr the output address
   * @param size the size of bytes
   * @return the size actually read
   */
  virtual size_t readBytes(void *ptr, size_t size) = 0;
};

/**
 * dakku output stream
 */
class OutputStream {
 public:
  /**
   * write a byte
   * @param value the value of the byte
   */
  virtual void writeByte(std::uint8_t value) = 0;
  /**
   * write bytes
   * @param ptr the start address of the bytes to write
   * @param size the size of bytes
   * @return the size actually write
   */
  virtual size_t writeBytes(const void *ptr, size_t size) = 0;
};
DAKKU_END
#endif  // DAKKU_SRC_CORE_STREAM_H_
