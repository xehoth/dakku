#include <core/fstream.h>
#include <core/relative.h>

DAKKU_BEGIN

FileInputStream::FileInputStream(const std::filesystem::path &path)
    : stream(path.is_absolute() ? path : RelativeRoot::get() + path.string()) {}

std::uint8_t FileInputStream::readByte() { return stream.rdbuf()->sbumpc(); }

size_t FileInputStream::readBytes(void *ptr, size_t size) {
  return stream.rdbuf()->sgetn(reinterpret_cast<char *>(ptr),
                               static_cast<std::streamsize>(size));
}

FileOutputStream::FileOutputStream(const std::filesystem::path &path)
    : stream(path.is_absolute() ? path : RelativeRoot::get() + path.string()) {}

void FileOutputStream::writeByte(std::uint8_t value) {
  stream.rdbuf()->sputc(static_cast<char>(value));
}
size_t FileOutputStream::writeBytes(void *ptr, size_t size) {
  return stream.rdbuf()->sputn(reinterpret_cast<char *>(ptr),
                               static_cast<std::streamsize>(size));
}

DAKKU_END
