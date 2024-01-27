#include "FileStream.h"

using namespace ntdv;

size_t FileStream::write(const uint8_t byte) {
  if (!*this) return 0;
  return _ptr->Write(&byte, 1);
}

time_t FileStream::GetLastWrite() const {
  if (!*this) return 0;
  return _ptr->GetLastWrite();
}

size_t FileStream::write(const uint8_t *buf, const size_t size) {
  if (!*this) return 0;
  return _ptr->Write(buf, size);
}

int FileStream::available() {
  if (!*this) return false;
  return _ptr->Size() - _ptr->Position();
}

int FileStream::read() {
  if (!*this) return -1;

  uint8_t result;
  if (_ptr->Read(&result, 1) != 1) return -1;
  return result;
}

size_t FileStream::Read(uint8_t* buf, const size_t size) const {
  if (!*this) return -1;
  return _ptr->Read(buf, size);
}

int FileStream::peek() {
  if (!*this) return -1;

  const size_t curPos = _ptr->Position();
  const int result = read();
  // ReSharper disable once CppExpressionWithoutSideEffects
  Seek(curPos, SeekSet);
  return result;
}

void FileStream::flush() {
  if (!*this) return;
  _ptr->Flush();
}

bool FileStream::Seek(const uint32_t pos, const SeekMode mode) const {
  if (!*this) return false;
  return _ptr->Seek(pos, mode);
}

size_t FileStream::Position() const {
  if (!*this) return -1;
  return _ptr->Position();
}

size_t FileStream::Size() const {
  if (!*this) return -1;
  return _ptr->Size();
}

bool FileStream::SetBufferSize(const size_t size) const {
  if (!*this) return false;
  return _ptr->SetBufferSize(size);
}

void FileStream::Close() {
  if (_ptr) {
    _ptr->Close();
    _ptr = nullptr;
  }
}

FileStream::operator bool() const {
  return _ptr != nullptr && *_ptr != false;
}

const char* FileStream::Path() const {
  if (!*this) return nullptr;
  return _ptr->Path();
}

const char* FileStream::Name() const {
  if (!*this) return nullptr;
  return _ptr->Name();
}

boolean FileStream::IsDirectory() const {
  if (!*this) return false;
  return _ptr->IsDirectory();
}

FileStream FileStream::OpenNextFile(const char* mode) const {
  if (!*this) return FileStream();
  return _ptr->OpenNextFile(mode);
}

boolean FileStream::SeekDir(const long position) const {
  if (!_ptr) return false;
  return _ptr->SeekDir(position);
}

String FileStream::GetNextFileName() const {
  if (!_ptr) return "";
  return _ptr->GetNextFileName();
}

String FileStream::GetNextFileName(boolean *isDir) const {
  if (!_ptr) return "";
  return _ptr->GetNextFileName(isDir);
}

void FileStream::RewindDirectory() const {
  if (!*this) return;
  _ptr->RewindDirectory();
}