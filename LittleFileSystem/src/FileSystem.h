#pragma once

#include "IFileSystem.h"
#include "File.h"

namespace ntdv {
class FileSystem {
  public:
  FileSystem(FileSystemPtr impl) : _impl(impl) { }

  File Open(const char* path, const char* mode = FILE_READ, const bool create = false);
  File Open(const String& path, const char* mode = FILE_READ, const bool create = false);

  bool Exists(const char* path);
  bool Exists(const String& path);

  bool Remove(const char* path);
  bool Remove(const String& path);

  bool Rename(const char* pathFrom, const char* pathTo);
  bool Rename(const String& pathFrom, const String& pathTo);

  bool CreateDirectory(const char *path);
  bool CreateDirectory(const String &path);

  bool RemoveDirectory(const char *path);
  bool RemoveDirectory(const String &path);

  const char * Mountpoint();

  protected:
  FileSystemPtr _impl;
};
}