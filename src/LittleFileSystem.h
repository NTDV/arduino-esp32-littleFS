#pragma once

#include "FileSystem.h"

namespace ntdv {
class LittleFileSystem : public FileSystem {
  public:
  LittleFileSystem();
  ~LittleFileSystem();

  bool Begin(bool formatOnFail = false, const char * basePath = "/littlefs", const char * partitionLabel = "spiffs");
  bool Format();

  size_t TotalBytes();
  size_t UsedBytes();

  void End();

  private:
  char * _partitionLabel;
};
} // namespace ntdv

extern ntdv::LittleFileSystem LittleFS;