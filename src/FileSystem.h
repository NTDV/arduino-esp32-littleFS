#pragma once

#include <utility>

#include "FileStream.h"
#include "IFileSystem.h"

namespace ntdv {

/// FileSystem class provides a high-level interface for file system operations
class FileSystem {
  public:
  /// Constructor
  /// @param impl - shared pointer to an IFileSystem object
  FileSystem(FileSystemPtr impl) : _impl(std::move(impl)) { }

  /// Open a file
  /// @param path - path of the file
  /// @param mode - mode to open the file in (default is read mode)
  /// @param create - whether to create the file if it doesn't exist (default is false)
  /// @return a FileStream object for the opened file
  FileStream Open(const char* path, const char* mode = FILE_READ, bool create = false) const;

  /// Open a file
  /// @param path - path of the file
  /// @param mode - mode to open the file in (default is read mode)
  /// @param create - whether to create the file if it doesn't exist (default is false)
  /// @return a FileStream object for the opened file
  FileStream Open(const String& path, const char* mode = FILE_READ, bool create = false) const;

  /// Check if a file exists
  /// @param path - path of the file
  /// @return true if the file exists, false otherwise
  bool Exists(const char* path) const;

  /// Check if a file exists
  /// @param path - path of the file
  /// @return true if the file exists, false otherwise
  bool Exists(const String& path) const;

  /// Remove a file
  /// @param path - path of the file
  /// @return true if successful, false otherwise
  bool Remove(const char* path) const;

  /// Remove a file
  /// @param path - path of the file
  /// @return true if successful, false otherwise
  bool Remove(const String& path) const;

  /// Rename a file
  /// @param pathFrom - current path of the file
  /// @param pathTo - new path of the file
  /// @return true if successful, false otherwise
  bool Rename(const char* pathFrom, const char* pathTo) const;

  /// Rename a file
  /// @param pathFrom - current path of the file
  /// @param pathTo - new path of the file
  /// @return true if successful, false otherwise
  bool Rename(const String& pathFrom, const String& pathTo) const;

  /// Create a directory
  /// @param path - path of the directory
  /// @return true if successful, false otherwise
  bool CreateDirectory(const char *path) const;

  /// Create a directory
  /// @param path - path of the directory
  /// @return true if successful, false otherwise
  bool CreateDirectory(const String &path) const;

  /// Remove a directory
  /// @param path - path of the directory
  /// @return true if successful, false otherwise
  bool RemoveDirectory(const char *path) const;

  /// Remove a directory
  /// @param path - path of the directory
  /// @return true if successful, false otherwise
  bool RemoveDirectory(const String &path) const;

  /// Get the mount point of the file system
  /// @return the mount point
  const char * Mountpoint() const;

  protected:
  /// Shared pointer to an IFileSystem object
  FileSystemPtr _impl;
};
} // namespace ntdv