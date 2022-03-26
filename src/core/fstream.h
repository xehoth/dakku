#ifndef DAKKU_SRC_CORE_FSTREAM_H_
#define DAKKU_SRC_CORE_FSTREAM_H_
#include <core/stream.h>
#include <fstream>
#include <filesystem>

DAKKU_BEGIN
/**
 * dakku file input stream
 */
class FileInputStream : public InputStream {
 public:
  /**
   * @param path the path of the file
   */
  explicit FileInputStream(const std::filesystem::path &path);
  /**
   * @return one byte read from the stream
   */
  std::uint8_t readByte() override;
  /**
   * read [size] bytes into [ptr]
   * @param ptr the address of the buffer
   * @param size the number of bytes to read
   * @return the actual number of bytes that the stream reads
   */
  size_t readBytes(void *ptr, size_t size) override;

 protected:
  std::ifstream stream;
};

/**
 * dakku file output stream
 */
class FileOutputStream : public OutputStream {
 public:
  /**
   * @param path the path of the file
   */
  explicit FileOutputStream(const std::filesystem::path &path);
  /**
   * write one byte
   * @param value the byte to write
   */
  void writeByte(std::uint8_t value) override;
  /**
   * write [size] bytes starting at [ptr]
   * @param ptr the starting pointer (buffer address)
   * @param size the number of bytes
   * @return the actual number of bytes the stream writes
   */
  size_t writeBytes(const void *ptr, size_t size) override;

 protected:
  std::ofstream stream;
};

DAKKU_END
#endif  // DAKKU_SRC_CORE_FSTREAM_H_
