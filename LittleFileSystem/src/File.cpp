#include "File.h"

using namespace ntdv;

size_t File::write(uint8_t c) {
  if (!*this) return 0;
  return _ptr->Write(&c, 1);
}

time_t File::GetLastWrite() {
  if (!*this) return 0;
  return _ptr->GetLastWrite();
}

size_t File::write(const uint8_t *buf, size_t size) {
  if (!*this) return 0;
  return _ptr->Write(buf, size);
}

int File::available() {
  if (!*this) return false;
  return _ptr->Size() - _ptr->Position();
}

int File::read() {
  if (!*this) return -1;

  uint8_t result;
  if (_ptr->Read(&result, 1) != 1) return -1;
  return result;
}

size_t File::Read(uint8_t* buf, size_t size) {
  if (!*this) return -1;
  return _ptr->Read(buf, size);
}

int File::peek() {
  if (!*this) return -1;

  size_t curPos = _ptr->Position();
  int result = read();
  Seek(curPos, SeekSet);
  return result;
}

void File::flush() {
  if (!*this) return;
  _ptr->Flush();
}

bool File::Seek(uint32_t pos, SeekMode mode) {
  if (!*this) return false;
  return _ptr->Seek(pos, mode);
}

size_t File::Position() const {
  if (!*this) return -1;
  return _ptr->Position();
}

size_t File::Size() const {
  if (!*this) return -1;
  return _ptr->Size();
}

bool File::SetBufferSize(size_t size) {
  if (!*this) return false;
  return _ptr->SetBufferSize(size);
}

void File::Close() {
  if (_ptr) {
    _ptr->Close();
    _ptr = nullptr;
  }
}

File::operator bool() const {
  return _ptr != nullptr && *_ptr != false;
}

const char* File::Path() const {
  if (!*this) return nullptr;
  return _ptr->Path();
}

const char* File::Name() const {
  if (!*this) return nullptr;
  return _ptr->Name();
}

boolean File::IsDirectory(void) {
  if (!*this) return false;
  return _ptr->IsDirectory();
}

File File::OpenNextFile(const char* mode) {
  if (!*this) return File();
  return _ptr->OpenNextFile(mode);
}

boolean File::SeekDir(long position) {
  if (!_ptr) return false;
  return _ptr->SeekDir(position);
}

String File::GetNextFileName(void) {
  if (!_ptr) return "";
  return _ptr->GetNextFileName();
}

String File::GetNextFileName(boolean *isDir) {
  if (!_ptr) return "";
  return _ptr->GetNextFileName(isDir);
}

void File::RewindDirectory(void) {
  if (!*this) return;
  _ptr->RewindDirectory();
}