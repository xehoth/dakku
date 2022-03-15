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
  std::uint8_t readByte() override;
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
  void writeByte(std::uint8_t value) override;
  size_t writeBytes(void *ptr, size_t size) override;

 protected:
  std::ofstream stream;
};

DAKKU_END
#endif  // DAKKU_SRC_CORE_FSTREAM_H_
