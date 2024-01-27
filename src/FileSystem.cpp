#include "FileSystem.h"

using namespace ntdv;

FileStream FileSystem::Open(const String& path, const char* mode, const bool create) const {
  return Open(path.c_str(), mode, create);
}

FileStream FileSystem::Open(const char* path, const char* mode, const bool create) const {
  if (!_impl) return FileStream();
  return FileStream(_impl->Open(path, mode, create));
}

bool FileSystem::Exists(const char* path) const {
  if (!_impl) return false;
  return _impl->Exists(path);
}

bool FileSystem::Exists(const String& path) const {
  return Exists(path.c_str());
}

bool FileSystem::Remove(const char* path) const {
  if (!_impl) return false;
  return _impl->Remove(path);
}

bool FileSystem::Remove(const String& path) const {
  return Remove(path.c_str());
}

bool FileSystem::Rename(const char* pathFrom, const char* pathTo) const {
  if (!_impl) return false;
  return _impl->Rename(pathFrom, pathTo);
}

bool FileSystem::Rename(const String& pathFrom, const String& pathTo) const {
  return Rename(pathFrom.c_str(), pathTo.c_str());
}

bool FileSystem::CreateDirectory(const char *path) const {
  if (!_impl) return false;
  return _impl->CreateDirectory(path);
}

bool FileSystem::CreateDirectory(const String &path) const {
  return CreateDirectory(path.c_str());
}

bool FileSystem::RemoveDirectory(const char *path) const {
  if (!_impl) return false;
  return _impl->RemoveDirectory(path);
}

bool FileSystem::RemoveDirectory(const String &path) const {
  return RemoveDirectory(path.c_str());
}

const char * FileSystem::Mountpoint() const {
  if (!_impl) return nullptr;
  return _impl->Mountpoint();
}