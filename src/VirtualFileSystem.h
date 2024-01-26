#pragma once

#include "IFileSystem.h"

namespace ntdv {
class VirtualFile;

class VirtualFileSystem : public IFileSystem {
  protected:
  friend class VirtualFile;

  public:
  FilePtr Open(const char* path, const char* mode, bool create) override;
  bool Exists(const char* path) override;
  bool Rename(const char* pathFrom, const char* pathTo) override;
  bool Remove(const char* path) override;
  bool CreateDirectory(const char *path) override;
  bool RemoveDirectory(const char *path) override;
};
}