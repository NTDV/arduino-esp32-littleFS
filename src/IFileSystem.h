#pragma once

#include "IFile.h"

namespace ntdv {

/// Interface for a file system
class IFileSystem {
  protected:
  /// Mount point of the file system
  const char *_mountpoint;

  public:
  /// Default constructor
  IFileSystem() : _mountpoint(nullptr) {}

  /// Virtual destructor
  virtual ~IFileSystem() {}

  /// Open a file
  /// @param path - path of the file
  /// @param mode - mode to open the file in
  /// @param create - whether to create the file if it doesn't exist
  /// @return a shared pointer to the opened file
  virtual FilePtr Open(const char *path, const char *mode, bool create) = 0;

  /// Check if a file exists
  /// @param path - path of the file
  /// @return true if the file exists, false otherwise
  virtual bool Exists(const char *path) = 0;

  /// Rename a file
  /// @param pathFrom - current path of the file
  /// @param pathTo - new path of the file
  /// @return true if successful, false otherwise
  virtual bool Rename(const char *pathFrom, const char *pathTo) = 0;

  /// Remove a file
  /// @param path - path of the file
  /// @return true if successful, false otherwise
  virtual bool Remove(const char *path) = 0;

  /// Create a directory
  /// @param path - path of the directory
  /// @return true if successful, false otherwise
  virtual bool CreateDirectory(const char *path) = 0;

  /// Remove a directory
  /// @param path - path of the directory
  /// @return true if successful, false otherwise
  virtual bool RemoveDirectory(const char *path) = 0;

  /// Set the mount point of the file system
  /// @param mountpoint - the mount point
  void Mountpoint(const char *mountpoint);

  /// Get the mount point of the file system
  /// @return the mount point
  const char *Mountpoint() const;
};

/// Define a shared pointer to an IFileSystem object
typedef std::shared_ptr<IFileSystem> FileSystemPtr;

} // namespace ntdv