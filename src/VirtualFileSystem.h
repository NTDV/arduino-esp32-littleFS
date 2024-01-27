#pragma once

#include "IFileSystem.h"

namespace ntdv {

class VirtualFile;

/// VirtualFileSystem class provides an interface for file system operations in a virtual file system
class VirtualFileSystem : public IFileSystem {
  protected:
  /// Friend class declaration to allow VirtualFile to access protected members of VirtualFileSystem
  friend class VirtualFile;

  public:
  /// Open a file
  /// @param path - path of the file
  /// @param mode - mode to open the file in
  /// @param create - whether to create the file if it doesn't exist
  /// @return a shared pointer to the opened file
  FilePtr Open(const char* path, const char* mode, bool create) override;

  /// Check if a file exists
  /// @param path - path of the file
  /// @return true if the file exists, false otherwise
  bool Exists(const char* path) override;

  /// Rename a file
  /// @param pathFrom - current path of the file
  /// @param pathTo - new path of the file
  /// @return true if successful, false otherwise
  bool Rename(const char* pathFrom, const char* pathTo) override;

  /// Remove a file
  /// @param path - path of the file
  /// @return true if successful, false otherwise
  bool Remove(const char* path) override;

  /// Create a directory
  /// @param path - path of the directory
  /// @return true if successful, false otherwise
  bool CreateDirectory(const char *path) override;

  /// Remove a directory
  /// @param path - path of the directory
  /// @return true if successful, false otherwise
  bool RemoveDirectory(const char *path) override;
};
} // namespace ntdv