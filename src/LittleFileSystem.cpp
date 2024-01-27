#include "VirtualFile.h"
#include "LittleFileSystem.h"
#include "FileSystem.h"

#define CONFIG_LITTLEFS_PAGE_SIZE

#ifdef CONFIG_LITTLEFS_PAGE_SIZE
extern "C" {
#include "esp_littlefs.h"
}

using namespace ntdv;

class LittleFileSystemImpl final : public VirtualFileSystem {
  public:
  LittleFileSystemImpl() = default;
  ~LittleFileSystemImpl() override = default;
  bool Exists(const char *path) override;
};

bool LittleFileSystemImpl::Exists(const char *path) {
  const FileStream file = Open(path, "r", false);
  return file == true;
}

LittleFileSystem::LittleFileSystem() : FileSystem(std::make_shared<LittleFileSystemImpl>()),
_partitionLabel(NULL) { }

LittleFileSystem::~LittleFileSystem() {
  if (_partitionLabel) {
    free(_partitionLabel);
    _partitionLabel = NULL;
  }
}

bool LittleFileSystem::Begin(const bool formatOnFail, const char *basePath, const char *partitionLabel) {
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
  // };

  const esp_vfs_littlefs_conf_t conf = {
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

void LittleFileSystem::End() const {
  if(esp_littlefs_mounted(_partitionLabel)){
    const esp_err_t err = esp_vfs_littlefs_unregister(_partitionLabel);

    if(err){
      log_e("Unmounting LittleFS failed! Error: %d", err);
      return;
    }

    _impl->Mountpoint(NULL);
  }
}

bool LittleFileSystem::Format() const {
  disableCore0WDT(); // Отключение сторожевого таймера, т.к. форматирование - долгая операция
  const esp_err_t err = esp_littlefs_format(_partitionLabel);
  enableCore0WDT(); // Включение сторожевого таймера

  if(err){
    log_e("Formatting LittleFS failed! Error: %d", err);
    return false;
  }
  return true;
}

size_t LittleFileSystem::TotalBytes() const {
  size_t total,used;
  if(esp_littlefs_info(_partitionLabel, &total, &used)){
    return 0;
  }
  return total;
}

size_t LittleFileSystem::UsedBytes() const {
  size_t total,used;
  if(esp_littlefs_info(_partitionLabel, &total, &used)){
    return 0;
  }
  return used;
}

LittleFileSystem LittleFS;

#endif
