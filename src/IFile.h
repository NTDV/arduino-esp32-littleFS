#pragma once

#include <Arduino.h>
#include <cstddef>
#include <memory>
#include <cstdint>

namespace ntdv {
#define FILE_READ   "r"
#define FILE_WRITE  "w"
#define FILE_APPEND "a"

enum SeekMode {
  SeekSet = 0,
  SeekCur = 1,
  SeekEnd = 2
};

class IFile;
typedef std::shared_ptr<IFile> FilePtr;

class IFile {
  public:
  virtual ~IFile() = default;

  virtual size_t Write(const uint8_t *buf, size_t size) = 0;
  virtual size_t Read(uint8_t *buf, size_t size) = 0;

  virtual void Flush() = 0;
  virtual bool Seek(uint32_t pos, SeekMode mode) = 0;
  [[nodiscard]] virtual size_t Position() const = 0;

  [[nodiscard]] virtual size_t Size() const = 0;
  virtual bool SetBufferSize(size_t size) = 0;

  virtual void Close() = 0;
  virtual time_t GetLastWrite() = 0;

  [[nodiscard]] virtual const char *Path() const = 0;
  [[nodiscard]] virtual const char *Name() const = 0;

  virtual boolean IsDirectory() = 0;
  virtual FilePtr OpenNextFile(const char *mode) = 0;
  virtual boolean SeekDir(long position) = 0;

  virtual String GetNextFileName() = 0;
  virtual String GetNextFileName(bool *isDir) = 0;

  virtual void RewindDirectory() = 0;

  virtual operator bool() = 0;
};
} // namespace ntdv