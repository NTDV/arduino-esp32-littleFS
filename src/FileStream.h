#pragma once

#include "IFile.h"

namespace ntdv {

/// FileStream class inherits from Stream class
class FileStream : public Stream {
  public:
  /// Constructor
  /// @param ptr - shared pointer to an IFile object
  FileStream(const FilePtr &ptr = FilePtr()) : _ptr(ptr) {
    _timeout = 0;
  }

  /// Write a byte to the stream
  /// @param byte - the byte to write
  /// @return the number of bytes written
  size_t write(uint8_t byte) override;

  /// Write data to the stream
  /// @param buf - buffer containing the data
  /// @param size - size of the data
  /// @return the number of bytes written
  size_t write(const uint8_t *buf, size_t size) override;

  /// Check the number of bytes available for reading
  /// @return the number of bytes available
  int available() override;

  /// Read a byte from the stream
  /// @return the byte read
  int read() override;

  /// Peek at the next byte in the stream
  /// @return the next byte
  int peek() override;

  /// Flush the stream
  void flush() override;

  /// Read data from the stream
  /// @param buf - buffer to store the data
  /// @param size - size of the data to read
  /// @return the number of bytes read
  size_t Read(uint8_t* buf, size_t size) const;

  /// Read data from the stream
  /// @param buffer - buffer to store the data
  /// @param length - size of the data to read
  /// @return the number of bytes read
  size_t ReadBytes(char *buffer, const size_t length) const {
    return Read(reinterpret_cast<uint8_t *>(buffer), length);
  }

  /// Set a position in the stream
  /// @param pos - position to seek to
  /// @param mode - mode of seeking
  /// @return true if successful, false otherwise
  bool Seek(uint32_t pos, SeekMode mode) const;

  /// Seek to a position in the stream
  /// @param pos - position to seek to
  /// @return true if successful, false otherwise
  bool Seek(const uint32_t pos) const {
    return Seek(pos, SeekSet);
  }

  /// Get the current position in the stream
  /// @return the current position
  [[nodiscard]] size_t Position() const;

  /// Get the size of the stream
  /// @return the size of the stream
  [[nodiscard]] size_t Size() const;

  /// Set the buffer size for the stream
  /// @param size - size of the buffer
  /// @return true if successful, false otherwise
  [[nodiscard]] bool SetBufferSize(size_t size) const;

  /// Close the stream
  void Close();

  /// Check if the stream is valid
  /// @return true if the stream is valid, false otherwise
  operator bool() const;

  /// Get the last write time of the stream
  /// @return the last write time
  [[nodiscard]] time_t GetLastWrite() const;

  /// Get the path of the stream
  /// @return the stream path
  [[nodiscard]] const char* Path() const;

  /// Get the name of the stream
  /// @return the stream name
  [[nodiscard]] const char* Name() const;

  /// Check if the stream is a directory
  /// @return true if the stream is a directory, false otherwise
  [[nodiscard]] boolean IsDirectory() const;

  /// Seek to a position in the directory
  /// @param position - position to seek to
  /// @return true if successful, false otherwise
  [[nodiscard]] boolean SeekDir(long position) const;

  /// Open the next file in the directory
  /// @param mode - mode to open the file in
  /// @return a FileStream object for the opened file
  FileStream OpenNextFile(const char* mode = FILE_READ) const;

  /// Get the name of the next file in the directory
  /// @return the name of the next file
  [[nodiscard]] String GetNextFileName() const;

  /// Get the name of the next file in the directory and whether it is a directory
  /// @param isDir - pointer to a boolean to store whether the file is a directory
  /// @return the name of the next file
  String GetNextFileName(boolean *isDir) const;

  /// Go to the begining of the directory
  void RewindDirectory() const;

  protected:
  /// Shared pointer to an IFile object
  FilePtr _ptr;
};
} // namespace ntdv