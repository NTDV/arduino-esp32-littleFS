#include "VirtualFile.h"
#include "LittleFileSystem.h"
#include "FileSystem.h"

#define CONFIG_LITTLEFS_PAGE_SIZE

#ifdef CONFIG_LITTLEFS_PAGE_SIZE
extern "C" {
#include "esp_littlefs.h"
}

using namespace ntdv;

class LittleFileSystemImpl : public VirtualFileSystem {
  public:
  LittleFileSystemImpl();
  virtual ~LittleFileSystemImpl() { }
  virtual bool Exists(const char* path);
};

LittleFileSystemImpl::LittleFileSystemImpl() {}

bool LittleFileSystemImpl::Exists(const char *path) {
  File file = Open(path, "r", false);
  return file == true;
}

LittleFileSystem::LittleFileSystem() : FileSystem(FileSystemPtr(new LittleFileSystemImpl())),
_partitionLabel(NULL) { }

LittleFileSystem::~LittleFileSystem() {
  if (_partitionLabel) {
    free(_partitionLabel);
    _partitionLabel = NULL;
  }
}

bool LittleFileSystem::Begin(bool formatOnFail, const char *basePath, uint8_t maxOpenFiles, const char *partitionLabel) {
  if (_partitionLabel) {
    free(_partitionLabel);
    _partitionLabel = NULL;
  }

  if (partitionLabel) {
    _partitionLabel = strdup(partitionLabel);
  }

  if(esp_littlefs_mounted(_partitionLabel)){
    log_w("LittleFS Already Mounted!");
    return true;
  }

  //esp_vfs_littlefs_conf_t conf = {
  //  .base_path = basePath,
  //  .partition_label = _partitionLabel,
  //  .partition = NULL,
  //  .format_if_mount_failed = false,
  //  .read_only = false,
  //  .dont_mount = false,
  //  .grow_on_mount = true
  //};

  esp_vfs_littlefs_conf_t conf = {
    .base_path = basePath,
    .partition_label = _partitionLabel,
    .format_if_mount_failed = false,
    .dont_mount = false
  };

  esp_err_t err = esp_vfs_littlefs_register(&conf);
  if(err == ESP_FAIL && formatOnFail){
    if(Format()){
      err = esp_vfs_littlefs_register(&conf);
    }
  }
  if(err != ESP_OK){
    log_e("Mounting LittleFS failed! Error: %d", err);
    return false;
  }
  _impl->Mountpoint(basePath);
  return true;
}

void LittleFileSystem::End() {
  if(esp_littlefs_mounted(_partitionLabel)){
    esp_err_t err = esp_vfs_littlefs_unregister(_partitionLabel);
    if(err){
      log_e("Unmounting LittleFS failed! Error: %d", err);
      return;
    }
    _impl->Mountpoint(NULL);
  }
}

bool LittleFileSystem::Format() {
  disableCore0WDT();
  esp_err_t err = esp_littlefs_format(_partitionLabel);
  enableCore0WDT();
  if(err){
    log_e("Formatting LittleFS failed! Error: %d", err);
    return false;
  }
  return true;
}

size_t LittleFileSystem::TotalBytes() {
  size_t total,used;
  if(esp_littlefs_info(_partitionLabel, &total, &used)){
    return 0;
  }
  return total;
}

size_t LittleFileSystem::UsedBytes() {
  size_t total,used;
  if(esp_littlefs_info(_partitionLabel, &total, &used)){
    return 0;
  }
  return used;
}

LittleFileSystem LittleFS;

#endif
