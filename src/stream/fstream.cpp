#include <stream/fstream.h>
#include <core/relative.h>

namespace dakku {

FileInputStream::FileInputStream(const std::filesystem::path &path)
    : stream(path.is_absolute()
                 ? path
                 : std::filesystem::path(RelativeRoot::instance().get() +
                                         path.string())) {}

std::uint8_t FileInputStream::readByte() {
  return static_cast<std::uint8_t>(stream.rdbuf()->sbumpc());
}

size_t FileInputStream::readBytes(void *ptr, size_t size) {
  return stream.rdbuf()->sgetn(reinterpret_cast<char *>(ptr),
                               static_cast<std::streamsize>(size));
}

FileOutputStream::FileOutputStream(const std::filesystem::path &path)
    : stream(path.is_absolute()
                 ? path
                 : std::filesystem::path(RelativeRoot::instance().get() +
                                         path.string())) {}

void FileOutputStream::writeByte(std::uint8_t value) {
  stream.rdbuf()->sputc(static_cast<char>(value));
}
size_t FileOutputStream::writeBytes(const void *ptr, size_t size) {
  return stream.rdbuf()->sputn(reinterpret_cast<const char *>(ptr),
                               static_cast<std::streamsize>(size));
}

FileInputStream::FileInputStream(const Property &p)
    : FileInputStream(std::filesystem::path{p.getString()}) {}

FileOutputStream::FileOutputStream(const Property &p)
    : FileOutputStream(std::filesystem::path{p.getString()}) {}

}  // namespace dakku