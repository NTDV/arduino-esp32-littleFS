#include "IFileSystem.h"

using namespace ntdv;

void IFileSystem::Mountpoint(const char * mountpoint) {
  _mountpoint = mountpoint;
}

const char * IFileSystem::Mountpoint() const {
  return _mountpoint;
}
