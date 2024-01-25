#pragma once

#include "IFile.h"

namespace ntdv {
class File : public Stream {
  public:
  File(FilePtr ptr = FilePtr()) : _ptr(ptr) {
    _timeout = 0;
  }

  size_t write(uint8_t) override;
  size_t write(const uint8_t *buf, size_t size) override;
  int available() override;
  int read() override;
  int peek() override;
  void flush() override;

  size_t Read(uint8_t* buf, size_t size);
  size_t ReadBytes(char *buffer, size_t length) {
    return Read((uint8_t*)buffer, length);
  }

  bool Seek(uint32_t pos, SeekMode mode);
  bool Seek(uint32_t pos) {
    return Seek(pos, SeekSet);
  }
  size_t Position() const;
  size_t Size() const;
  bool SetBufferSize(size_t size);
  void Close();
  operator bool() const;
  time_t GetLastWrite();
  const char* Path() const;
  const char* Name() const;

  boolean IsDirectory(void);
  boolean SeekDir(long position);
  File OpenNextFile(const char* mode = FILE_READ);
  String GetNextFileName(void);
  String GetNextFileName(boolean *isDir);
  void RewindDirectory(void);

  protected:
  FilePtr _ptr;
};
}