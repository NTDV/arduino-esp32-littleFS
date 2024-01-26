#pragma once

#include "IFile.h"

namespace ntdv {
class IFileSystem {
  protected:
  const char *_mountpoint;

  public:
  IFileSystem() : _mountpoint(nullptr) {}
  virtual ~IFileSystem() {}

  virtual FilePtr Open(const char *path, const char *mode, bool create) = 0;

  virtual bool Exists(const char *path) = 0;
  virtual bool Rename(const char *pathFrom, const char *pathTo) = 0;
  virtual bool Remove(const char *path) = 0;

  virtual bool CreateDirectory(const char *path) = 0;
  virtual bool RemoveDirectory(const char *path) = 0;

  void Mountpoint(const char *);
  const char *Mountpoint();
};

typedef std::shared_ptr<IFileSystem> FileSystemPtr;
} // namespace ntdv