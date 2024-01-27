#pragma once

#include "FileSystem.h"

namespace ntdv {

/// LittleFileSystem class provides a high-level interface for LittleFS file system operations
class LittleFileSystem : public FileSystem {
  public:
  /// Default constructor
  LittleFileSystem();

  /// Destructor
  ~LittleFileSystem();

  /// Initialize the file system
  /// @param formatOnFail - whether to format the file system if initialization fails (default is false)
  /// @param basePath - base path for the file system (default is "/littlefs")
  /// @param partitionLabel - label for the partition (default is "spiffs")
  /// @return true if successful, false otherwise
  bool Begin(bool formatOnFail = false, const char * basePath = "/littlefs", const char * partitionLabel = "spiffs");

  /// Format the file system
  /// @return true if successful, false otherwise
  bool Format() const;

  /// Get the total size of the file system
  /// @return the total size in bytes
  size_t TotalBytes() const;

  /// Get the used size of the file system
  /// @return the used size in bytes
  size_t UsedBytes() const;

  /// Unmount the file system
  void End() const;

  private:
  /// Label for the partition
  char * _partitionLabel;
};

} // namespace ntdv

/// Global instance of the LittleFileSystem class
extern ntdv::LittleFileSystem LittleFS;