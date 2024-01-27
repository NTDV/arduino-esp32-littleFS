#pragma once

#include <Arduino.h>
#include <cstddef>
#include <memory>
#include <cstdint>

namespace ntdv {
/// File access modes
#define FILE_READ   "r" /// Read mode
#define FILE_WRITE  "w" /// Write mode
#define FILE_APPEND "a" /// Append mode

/// Enum for seek modes
enum SeekMode {
  SeekSet = 0,  /// Seek from the beginning of the file
  SeekCur = 1,  /// Seek from the current position
  SeekEnd = 2   /// Seek from the end of the file
};

class IFile;
/// Define a shared pointer to an IFile object
typedef std::shared_ptr<IFile> FilePtr;

/// Interface for a file
class IFile {
  public:
  /// Virtual destructor
  virtual ~IFile() = default;

  /// Write data to the file
  /// @param buf - buffer containing the data
  /// @param size - size of the data
  /// @return the number of bytes written
  virtual size_t Write(const uint8_t *buf, size_t size) = 0;

  /// Read data from the file
  /// @param buf - buffer to store the data
  /// @param size - size of the data to read
  /// @return the number of bytes read
  virtual size_t Read(uint8_t *buf, size_t size) = 0;

  /// Flush the file buffer
  virtual void Flush() = 0;

  /// Set a position in the file
  /// @param pos - position to seek to
  /// @param mode - mode of seeking
  /// @return true if successful, false otherwise
  virtual bool Seek(uint32_t pos, SeekMode mode) = 0;

  /// Get the current position in the file
  /// @return the current position
  [[nodiscard]] virtual size_t Position() const = 0;

  /// Get the size of the file
  /// @return the size of the file
  [[nodiscard]] virtual size_t Size() const = 0;

  /// Set the buffer size for the file
  /// @param size - size of the buffer
  /// @return true if successful, false otherwise
  virtual bool SetBufferSize(size_t size) = 0;

  /// Close the file
  virtual void Close() = 0;

  /// Get the last write time of the file
  /// @return the last write time
  virtual time_t GetLastWrite() = 0;

  /// Get the path of the file
  /// @return the file path
  [[nodiscard]] virtual const char *Path() const = 0;

  /// Get the name of the file
  /// @return the file name
  [[nodiscard]] virtual const char *Name() const = 0;

  /// Check if the file is a directory
  /// @return true if the file is a directory, false otherwise
  virtual boolean IsDirectory() = 0;

  /// Open the next file in the directory
  /// @param mode - mode to open the file in
  /// @return a shared pointer to the opened file
  virtual FilePtr OpenNextFile(const char *mode) = 0;

  /// Seek to a position in the directory
  /// @param position - position to seek to
  /// @return true if successful, false otherwise
  virtual boolean SeekDir(long position) = 0;

  /// Get the name of the next file in the directory
  /// @return the name of the next file
  virtual String GetNextFileName() = 0;

  /// Get the name of the next file in the directory and whether it is a directory
  /// @param isDir - pointer to a boolean to store whether the file is a directory
  /// @return the name of the next file
  virtual String GetNextFileName(bool *isDir) = 0;

  /// Go to the begining of the directory
  virtual void RewindDirectory() = 0;

  /// Check if the file is valid
  /// @return true if the file is valid, false otherwise
  virtual operator bool() = 0;
};
} // namespace ntdv