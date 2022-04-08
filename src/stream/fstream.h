#ifndef DAKKU_STREAM_FSTREAM_H_
#define DAKKU_STREAM_FSTREAM_H_
#include <stream/fwd.h>
#include <filesystem>
#include <fstream>

namespace dakku {
/**
 * @brief file input stream
 */
class DAKKU_EXPORT_STREAM FileInputStream : public InputStream {
 public:
  DAKKU_DECLARE_OBJECT(FileInputStream, InputStream);
  /**
   * @param path the path of the file
   */
  explicit FileInputStream(const std::filesystem::path &path);

  /**
   * @return one byte read from the stream
   */
  std::uint8_t readByte() override;

  /**
   * @brief read `size` bytes into `ptr`
   * @param ptr the address of the buffer
   * @param size the number of bytes to read
   * @return the actual number of bytes that the stream reads
   */
  size_t readBytes(void *ptr, size_t size) override;

 protected:
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  /// ifstream
  std::ifstream stream;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};

/**
 * @brief file output stream
 */
class DAKKU_EXPORT_STREAM FileOutputStream : public OutputStream {
 public:
  DAKKU_DECLARE_OBJECT(FileOutputStream, OutputStream);

  /**
   * @param path the path of the file
   */
  explicit FileOutputStream(const std::filesystem::path &path);
  /**
   * @brief write one byte
   * @param value the byte to write
   */
  void writeByte(std::uint8_t value) override;

  /**
   * @brief write `size` bytes starting at `ptr`
   * @param ptr the starting pointer (buffer address)
   * @param size the number of bytes
   * @return the actual number of bytes the stream writes
   */
  size_t writeBytes(const void *ptr, size_t size) override;

 protected:
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif
  /// ofstream
  std::ofstream stream;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
};
}  // namespace dakku
#endif