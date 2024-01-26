#pragma once

#include "IFile.h"
#include "VirtualFileSystem.h"

extern "C" {
#include <sys/stat.h>
#include <dirent.h>
}

namespace ntdv {
class VirtualFile : public IFile {
  protected:
  VirtualFileSystem* _fs;
  FILE* _file;
  DIR* _dir;
  char* _path;
  bool _isDirectory;
  mutable struct stat _stat;
  mutable bool _written;

  void getStat() const;

  public:
  VirtualFile(VirtualFileSystem* fs, const char* path, const char* mode);
  ~VirtualFile() override;

  size_t Write(const uint8_t *buf, size_t size) override;
  size_t Read(uint8_t* buf, size_t size) override;

  void Flush() override;
  bool Seek(uint32_t pos, SeekMode mode) override;
  size_t Position() const override;

  size_t Size() const override;
  bool SetBufferSize(size_t size) override;

  void Close() override;
  time_t GetLastWrite()  override;

  const char* Path() const override;
  const char* Name() const override;

  boolean IsDirectory(void) override;
  FilePtr OpenNextFile(const char* mode) override;
  boolean SeekDir(long position) override;

  String GetNextFileName(void) override;
  String GetNextFileName(bool *isDir) override;

  void RewindDirectory(void) override;

  operator bool() override;
};
}