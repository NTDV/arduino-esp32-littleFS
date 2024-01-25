#include "IFileSystem.h"

using namespace ntdv;

void IFileSystem::Mountpoint(const char * mp) {
  _mountpoint = mp;
}

const char * IFileSystem::Mountpoint() {
  return _mountpoint;
}
