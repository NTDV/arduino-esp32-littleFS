#include "VirtualFileSystem.h"
#include "VirtualFile.h"

using namespace ntdv;

FilePtr VirtualFileSystem::Open(const char* path, const char* mode, const bool create) {
  if(!_mountpoint) {
    log_e("File system is not mounted");
    return FilePtr();
  }

  if (!path || path[0] != '/') {
    log_e("%s does not start with /", path);
    return FilePtr();
  }

  const auto temp = static_cast<char *>(malloc(strlen(path) + strlen(_mountpoint) + 2));
  if(!temp) {
    log_e("malloc failed");
    return FilePtr();
  }

  strcpy(temp, _mountpoint);
  strcat(temp, path);

  struct stat st;

  // Файл найден
  if(!stat(temp, &st)) {
    free(temp);
    if (S_ISREG(st.st_mode) || S_ISDIR(st.st_mode)) {
      return std::make_shared<VirtualFile>(this, path, mode);
    }
    log_e("%s has wrong mode 0x%08X", path, st.st_mode);
    return FilePtr();
  }

  // Попытка открыть как папку (может быть точкой монтирования)
  DIR* dir = opendir(temp);
  if(dir) {
    closedir(dir);
    free(temp);
    return std::make_shared<VirtualFile>(this, path, mode);
  }

  // Файл не найден, но режим разрешает создать файл без создания папки
  if(mode && mode[0] != 'r' && !create){
    free(temp);
    return std::make_shared<VirtualFile>(this, path, mode);
  }

  // Файл не найден, но режим разрешает создать файл или папку
  if(mode && mode[0] != 'r' && create){
    const auto folder = static_cast<char *>(malloc(strlen(path)));

    constexpr int start_index = 0;
    int end_index = 0;

    const char* token = strchr(path+1,'/');
    end_index = (token - path);

    while (token != NULL) {
      memcpy(folder,path + start_index, end_index-start_index);
      folder[end_index-start_index] = '\0';

      if(!VirtualFileSystem::CreateDirectory(folder)) {
        log_e("Creating folder: %s failed!", folder);
        return FilePtr();
      }

      token = strchr(token+1,'/');
      if(token != NULL) {
        end_index = (token - path);
        memset(folder, 0, strlen(folder));
      }

    }

    free(folder);
    free(temp);
    return std::make_shared<VirtualFile>(this, path, mode);
  }

  log_e("%s does not exist, no permission for creation", temp);
  free(temp);
  return FilePtr();
}

bool VirtualFileSystem::Exists(const char* path) {
  if(!_mountpoint) {
    log_e("File system is not mounted");
    return false;
  }

  VirtualFile file(this, path, "r");
  if(file) {
    file.Close();
    return true;
  }

  return false;
}

bool VirtualFileSystem::Rename(const char* pathFrom, const char* pathTo){
  if(!_mountpoint) {
    log_e("File system is not mounted");
    return false;
  }

  if(!pathFrom || pathFrom[0] != '/' || !pathTo || pathTo[0] != '/') {
    log_e("bad arguments");
    return false;
  }

  if(!Exists(pathFrom)) {
    log_e("%s does not exists", pathFrom);
    return false;
  }

  const size_t mountpointLen = strlen(_mountpoint);
  const auto temp1 = static_cast<char *>(malloc(strlen(pathFrom) + mountpointLen + 1));
  if(!temp1) {
    log_e("malloc failed");
    return false;
  }
  const auto temp2 = static_cast<char *>(malloc(strlen(pathTo) + mountpointLen + 1));
  if(!temp2) {
    free(temp1);
    log_e("malloc failed");
    return false;
  }

  strcpy(temp1, _mountpoint);
  strcat(temp1, pathFrom);

  strcpy(temp2, _mountpoint);
  strcat(temp2, pathTo);

  const auto result = rename(temp1, temp2);

  free(temp1);
  free(temp2);

  return result == 0;
}

bool VirtualFileSystem::Remove(const char* path) {
  if(!_mountpoint) {
    log_e("File system is not mounted");
    return false;
  }

  if(!path || path[0] != '/') {
    log_e("bad arguments");
    return false;
  }

  VirtualFile file(this, path, "r");
  if(!file || file.IsDirectory()) {
    if(file) file.Close();
    log_e("%s does not exists or is directory", path);
    return false;
  }
  file.Close();

  const auto temp = static_cast<char *>(malloc(strlen(path) + strlen(_mountpoint) + 1));
  if(!temp) {
    log_e("malloc failed");
    return false;
  }

  strcpy(temp, _mountpoint);
  strcat(temp, path);

  const auto result = unlink(temp);

  free(temp);

  return result == 0;
}

bool VirtualFileSystem::CreateDirectory(const char *path){
  if(!_mountpoint) {
    log_e("File system is not mounted");
    return false;
  }

  VirtualFile file(this, path, "r");
  if(file && file.IsDirectory()) {
    file.Close();
    log_w("%s already exists", path);
    return true;
  }

  if(file) {
    file.Close();
    log_e("%s is a file", path);
    return false;
  }

  const auto temp = static_cast<char *>(malloc(strlen(path) + strlen(_mountpoint) + 1));
  if(!temp) {
    log_e("malloc failed");
    return false;
  }

  strcpy(temp, _mountpoint);
  strcat(temp, path);

  const auto result = mkdir(temp, ACCESSPERMS);

  free(temp);

  return result == 0;
}

bool VirtualFileSystem::RemoveDirectory(const char *path) {
  if(!_mountpoint) {
    log_e("File system is not mounted");
    return false;
  }

  if (strcmp(_mountpoint, "/spiffs") == 0) {
    log_e("rmdir is unnecessary in SPIFFS");
    return false;
  }

  VirtualFile file(this, path, "r");
  if(!file || !file.IsDirectory()) {
    if(file) file.Close();
    log_e("%s does not exists or is a file", path);
    return false;
  }
  file.Close();

  const auto temp = static_cast<char *>(malloc(strlen(path) + strlen(_mountpoint) + 1));
  if(!temp) {
    log_e("malloc failed");
    return false;
  }

  strcpy(temp, _mountpoint);
  strcat(temp, path);

  const auto result = rmdir(temp);

  free(temp);

  return result == 0;
}