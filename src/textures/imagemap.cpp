//
// Created by xehoth on 2021/10/7.
//
#include <dakku/textures/imagemap.h>

namespace dakku {

TexInfo::TexInfo(const std::string &fileName) : fileName(fileName) {}

bool TexInfo::operator<(const TexInfo &rhs) const {
  return fileName < rhs.fileName;
}

}  // namespace dakku