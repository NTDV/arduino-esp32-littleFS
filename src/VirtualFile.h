#pragma once

#include "IFile.h"
#include "VirtualFileSystem.h"

extern "C" {
#include <sys/stat.h>
#include <dirent.h>
}

namespace ntdv {

/// VirtualFile class provides an interface for file operations in a virtual file system
class VirtualFile : public IFile {
  protected:
  /// Pointer to the virtual file system
  VirtualFileSystem* _fs;

  /// Pointer to the file
  FILE* _file;

  /// Pointer to the directory
  DIR* _dir;

  /// Path of the file
  char* _path;

  /// Whether the file is a directory
  bool _isDirectory;

  /// File status information
  mutable struct stat _stat;

  /// Whether the file has been written to
  mutable bool _written;

  /// Get the file status information
  void getStat() const;

  public:
  /// Constructor
  /// @param fs - pointer to the virtual file system
  /// @param path - path of the file
  /// @param mode - mode to open the file in
  VirtualFile(VirtualFileSystem* fs, const char* path, const char* mode);

  /// Destructor
  ~VirtualFile() override;

  /// Write data to the file
  /// @param buf - buffer containing the data
  /// @param size - size of the data
  /// @return the number of bytes written
  size_t Write(const uint8_t *buf, size_t size) override;

  /// Read data from the file
  /// @param buf - buffer to store the data
  /// @param size - size of the data to read
  /// @return the number of bytes read
  size_t Read(uint8_t* buf, size_t size) override;

  /// Flush the file buffer
  void Flush() override;

  /// Seek to a position in the file
  /// @param pos - position to seek to
  /// @param mode - mode of seeking
  /// @return true if successful, false otherwise
  bool Seek(uint32_t pos, SeekMode mode) override;

  /// Get the current position in the file
  /// @return the current position
  size_t Position() const override;

  /// Get the size of the file
  /// @return the size of the file
  size_t Size() const override;

  /// Set the buffer size for the file
  /// @param size - size of the buffer
  /// @return true if successful, false otherwise
  bool SetBufferSize(size_t size) override;

  /// Close the file
  void Close() override;

  /// Get the last write time of the file
  /// @return the last write time
  time_t GetLastWrite()  override;

  /// Get the path of the file
  /// @return the file path
  const char* Path() const override;

  /// Get the name of the file
  /// @return the file name
  const char* Name() const override;

  /// Check if the file is a directory
  /// @return true if the file is a directory, false otherwise
  boolean IsDirectory() override;

  /// Open the next file in the directory
  /// @param mode - mode to open the file in
  /// @return a shared pointer to the opened file
  FilePtr OpenNextFile(const char* mode) override;

  /// Seek to a position in the directory
  /// @param position - position to seek to
  /// @return true if successful, false otherwise
  boolean SeekDir(long position) override;

  /// Get the name of the next file in the directory
  /// @return the name of the next file
  String GetNextFileName() override;

  /// Get the name of the next file in the directory and whether it is a directory
  /// @param isDir - pointer to a boolean to store whether the file is a directory
  /// @return the name of the next file
  String GetNextFileName(bool *isDir) override;

  /// Go to the begining of the directory
  void RewindDirectory() override;

  /// Check if the file is valid
  /// @return true if the file is valid, false otherwise
  operator bool() override;
};
} // namespace ntdv