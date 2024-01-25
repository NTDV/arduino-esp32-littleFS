#include "VirtualFile.h"
#include "VirtualFileSystem.h"

using namespace ntdv;

#define DEFAULT_FILE_BUFFER_SIZE 4096

VirtualFile::VirtualFile(VirtualFileSystem *fs, const char *path, const char *mode) :
    _fs(fs), _file(NULL), _dir(NULL), _path(NULL), _isDirectory(false), _written(false) {
  char *temp = static_cast<char *>(malloc(strlen(path) + strlen(_fs->_mountpoint) + 1));
  if (!temp)
    return;

  strcpy(temp, _fs->_mountpoint);
  strcat(temp, path);

  _path = strdup(path);
  if (!_path) {
    log_e("strdup(%s) failed", path);
    free(temp);
    return;
  }

  if (!stat(temp, &_stat)) {
    // Файл найден
    if (S_ISREG(_stat.st_mode)) {
      _isDirectory = false;
      _file = fopen(temp, mode);
      if (!_file) {
        log_e("fopen(%s) failed", temp);
      }
      if (_file && _stat.st_blksize == 0) {
        setvbuf(_file, NULL, _IOFBF, DEFAULT_FILE_BUFFER_SIZE);
      }
    } else if (S_ISDIR(_stat.st_mode)) {
      _isDirectory = true;
      _dir = opendir(temp);
      if (!_dir) {
        log_e("opendir(%s) failed", temp);
      }
    } else {
      log_e("Unknown type 0x%08X for file %s", ((_stat.st_mode) & _IFMT), temp);
    }
  } else {
    // Файл не найден
    if (!mode || mode[0] == 'r') {
      // Пробуем открыть как папку
      _dir = opendir(temp);
      if (_dir) {
        _isDirectory = true;
      } else {
        _isDirectory = false;
        log_w("stat(%s) failed", temp);
      }
    } else {
      // Создаем новый файл
      _isDirectory = false;
      _file = fopen(temp, mode);
      if (!_file) {
        log_e("fopen(%s) failed", temp);
      }
      if (_file && (_stat.st_blksize == 0)) {
        setvbuf(_file, NULL, _IOFBF, DEFAULT_FILE_BUFFER_SIZE);
      }
    }
  }
  free(temp);
}

VirtualFile::~VirtualFile() { Close(); }

void VirtualFile::Close() {
  if (_path) {
    free(_path);
    _path = NULL;
  }
  if (_isDirectory && _dir) {
    closedir(_dir);
    _dir = NULL;
    _isDirectory = false;
  } else if (_file) {
    fclose(_file);
    _file = NULL;
  }
}

VirtualFile::operator bool() { return (_isDirectory && _dir != NULL) || _file != NULL; }

time_t VirtualFile::GetLastWrite() {
  _getStat();
  return _stat.st_mtime;
}

void VirtualFile::_getStat() const {
  if (!_path)
    return;

  char *temp = static_cast<char *>(malloc(strlen(_path) + strlen(_fs->_mountpoint) + 1));
  if (!temp)
    return;

  strcpy(temp, _fs->_mountpoint);
  strcat(temp, _path);

  if (!stat(temp, &_stat)) {
    _written = false;
  }

  free(temp);
}

size_t VirtualFile::Write(const uint8_t *buf, size_t size) {
  if (_isDirectory || !_file || !buf || !size)
    return 0;

  _written = true;
  return fwrite(buf, 1, size, _file);
}

size_t VirtualFile::Read(uint8_t *buf, size_t size) {
  if (_isDirectory || !_file || !buf || !size)
    return 0;

  return fread(buf, 1, size, _file);
}

void VirtualFile::Flush() {
  if (_isDirectory || !_file)
    return;

  fflush(_file);
  fsync(fileno(_file));
}

bool VirtualFile::Seek(uint32_t pos, SeekMode mode) {
  if (_isDirectory || !_file)
    return false;

  auto result = fseek(_file, pos, mode);
  return result == 0;
}

size_t VirtualFile::Position() const {
  if (_isDirectory || !_file)
    return 0;

  return ftell(_file);
}

size_t VirtualFile::Size() const {
  if (_isDirectory || !_file)
    return 0;

  if (_written) {
    _getStat();
  }

  return _stat.st_size;
}

/*
 * Необходимо вызывать сразу после открытия файла до любых других операций!
 */
bool VirtualFile::SetBufferSize(size_t size) {
  if (_isDirectory || !_file)
    return false;

  int res = setvbuf(_file, NULL, _IOFBF, size);

  return res == 0;
}

const char *VirtualFile::Path() const { return _path; }

const char *VirtualFile::Name() const { return pathToFileName(Path()); }

boolean VirtualFile::IsDirectory(void) { return _isDirectory; }

FilePtr VirtualFile::OpenNextFile(const char *mode) {
  if (!_isDirectory || !_dir)
    return FilePtr();

  struct dirent *file = readdir(_dir);
  if (file == NULL) {
    return FilePtr();
  }
  // Если не файл и не папка, то пробуем еще раз
  if (file->d_type != DT_REG && file->d_type != DT_DIR) {
    return OpenNextFile(mode);
  }

  size_t pathLen = strlen(_path);
  size_t fileNameLen = strlen(file->d_name);
  char *name = static_cast<char *>(malloc(pathLen + fileNameLen + 2));

  if (name == NULL) {
    return FilePtr();
  }

  strcpy(name, _path);
  if (file->d_name[0] != '/' && _path[pathLen - 1] != '/') {
    strcat(name, "/");
  }
  strcat(name, file->d_name);

  FilePtr filePtr = std::make_shared<VirtualFile>(_fs, name, mode);

  free(name);

  return filePtr;
}

boolean VirtualFile::SeekDir(long position) {
  if (!_dir)
    return false;

  seekdir(_dir, position);

  return true;
}

String VirtualFile::GetNextFileName() {
  if (!_isDirectory || !_dir)
    return "";

  struct dirent *file = readdir(_dir);
  if (file == NULL) {
    return "";
  }
  // Если не файл и не папка, то нет пути к файлу
  if (file->d_type != DT_REG && file->d_type != DT_DIR) {
    return "";
  }

  String fname = String(file->d_name);
  String name = String(_path);
  if (!fname.startsWith("/") && !name.endsWith("/")) {
    name += "/";
  }
  name += fname;

  return name;
}

String VirtualFile::GetNextFileName(bool *isDir) {
  if (!_isDirectory || !_dir)
    return "";

  struct dirent *file = readdir(_dir);
  if (file == NULL) {
    return "";
  }
  if (file->d_type != DT_REG && file->d_type != DT_DIR) {
    return "";
  }
  String fname = String(file->d_name);
  String name = String(_path);
  if (!fname.startsWith("/") && !name.endsWith("/")) {
    name += "/";
  }
  name += fname;

  // Проверка на папку
  if (isDir) {
    *isDir = (file->d_type == DT_DIR);
  }

  return name;
}

void VirtualFile::RewindDirectory(void) {
  if (!_isDirectory || !_dir)
    return;

  rewinddir(_dir);
}
