#pragma once

#include "IFile.h"

namespace ntdv {
class FileStream : public Stream {
  public:
  FileStream(const FilePtr &ptr = FilePtr()) : _ptr(ptr) {
    _timeout = 0;
  }

  size_t write(uint8_t) override;
  size_t write(const uint8_t *buf, size_t size) override;
  int available() override;
  int read() override;
  int peek() override;
  void flush() override;

  size_t Read(uint8_t* buf, size_t size) const;
  size_t ReadBytes(char *buffer, const size_t length) const {
    return Read(reinterpret_cast<uint8_t *>(buffer), length);
  }

  bool Seek(uint32_t pos, SeekMode mode) const;
  bool Seek(const uint32_t pos) const {
    return Seek(pos, SeekSet);
  }
  [[nodiscard]] size_t Position() const;
  [[nodiscard]] size_t Size() const;
  [[nodiscard]] bool SetBufferSize(size_t size) const;
  void Close();
  operator bool() const;
  [[nodiscard]] time_t GetLastWrite() const;
  [[nodiscard]] const char* Path() const;
  [[nodiscard]] const char* Name() const;

  [[nodiscard]] boolean IsDirectory() const;
  [[nodiscard]] boolean SeekDir(long position) const;
  FileStream OpenNextFile(const char* mode = FILE_READ) const;
  [[nodiscard]] String GetNextFileName() const;
  String GetNextFileName(boolean *isDir) const;
  void RewindDirectory() const;

  protected:
  FilePtr _ptr;
};
}