#include <Arduino.h>
#include <LittleFileSystem.h>

// Перед запуском необходимо разметить пространтсво на мкироконтроллере плагином
//   https://github.com/lorol/arduino-esp32littlefs-plugin

// Для теста двух областей нужно разметиться согласно файлу partition.csv
//#define TWOPART

#define FORMAT_LITTLEFS_IF_FAILED true

void showDirectories(ntdv::FileSystem &fs, const char * dirname, const uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  const ntdv::FileStream root = fs.Open(dirname);
  if(!root){
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.IsDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  ntdv::FileStream file = root.OpenNextFile();
  while(file){
    if(file.IsDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.Name());
      if(levels){
        showDirectories(fs, file.Path(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.Name());
      Serial.print("\tSIZE: ");
      Serial.println(file.Size());
    }
    file = root.OpenNextFile();
  }
}

void createDirectory(ntdv::FileSystem &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.CreateDirectory(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDirectory(ntdv::FileSystem &fs, const char * path) {
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.RemoveDirectory(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(ntdv::FileSystem &fs, const char * path) {
  Serial.printf("Reading file: %s\r\n", path);

  ntdv::FileStream file = fs.Open(path);
  if(!file || file.IsDirectory()){
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.Close();
}

void writeFile(ntdv::FileSystem &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\r\n", path);

  ntdv::FileStream file = fs.Open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.Close();
}

void appendFile(ntdv::FileSystem &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\r\n", path);

  ntdv::FileStream file = fs.Open(path, FILE_APPEND);
  if(!file){
    Serial.println("- failed to open file for appending");
    return;
  }
  if(file.print(message)){
    Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.Close();
}

void renameFile(ntdv::FileSystem &fs, const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.Rename(path1, path2)) {
    Serial.println("- file renamed");
  } else {
    Serial.println("- rename failed");
  }
}

void deleteFile(ntdv::FileSystem &fs, const char * path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if(fs.Remove(path)){
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

// SPIFFS-like write and delete file, better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1

void writeFile_SPIFFS(ntdv::FileSystem &fs, const char * path, const char * message) {
  if(!fs.Exists(path)){
    if (strchr(path, '/')) {
      Serial.printf("Create missing folders of: %s\r\n", path);
      char *pathStr = strdup(path);
      if (pathStr) {
        char *ptr = strchr(pathStr, '/');
        while (ptr) {
          *ptr = 0;
          fs.CreateDirectory(pathStr);
          *ptr = '/';
          ptr = strchr(ptr+1, '/');
        }
      }
      free(pathStr);
    }
  }

  Serial.printf("Writing file to: %s\r\n", path);
  ntdv::FileStream file = fs.Open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.Close();
}

void deleteFile_SPIFFS(ntdv::FileSystem &fs, const char * path) {
  Serial.printf("Deleting file and empty folders on path: %s\r\n", path);

  if(fs.Remove(path)){
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }

  char *pathStr = strdup(path);
  if (pathStr) {
    char *ptr = strrchr(pathStr, '/');
    if (ptr) {
      Serial.printf("Removing all empty folders on path: %s\r\n", path);
    }
    while (ptr) {
      *ptr = 0;
      fs.RemoveDirectory(pathStr);
      ptr = strrchr(pathStr, '/');
    }
    free(pathStr);
  }
}

void benchIO(ntdv::FileSystem &fs, const char * path) {
  Serial.printf("Testing file I/O with %s\r\n", path);

  static uint8_t buf[512];
  ntdv::FileStream file = fs.Open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }

  size_t i;
  Serial.print("- writing" );
  uint32_t start = millis();
  for(i=0; i<2048; i++){
    if ((i & 0x001F) == 0x001F){
      Serial.print(".");
    }
    file.write(buf, 512);
  }
  Serial.println("");
  uint32_t end = millis() - start;
  Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
  file.Close();

  file = fs.Open(path);
  start = millis();
  end = start;
  i = 0;
  if(file && !file.IsDirectory()) {
    size_t len = 0;
    len = file.Size();
    const size_t flen = len;
    start = millis();
    Serial.print("- reading" );
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.Read(buf, toRead);
      if ((i++ & 0x001F) == 0x001F){
        Serial.print(".");
      }
      len -= toRead;
    }
    Serial.println("");
    end = millis() - start;
    Serial.printf("- %lu bytes read in %u ms\r\n", flen, end);
    file.Close();
  } else {
    Serial.println("- failed to open file for reading");
  }
}

void setup() {
  Serial.begin(115200);

  #ifdef TWOPART
  if(!LittleFS.Begin(FORMAT_LITTLEFS_IF_FAILED, "/lfs2", "part2")){
    Serial.println("part2 Mount Failed");
    return;
  }

  appendFile(LittleFS, "/hello0.txt", "World0!\r\n");
  readFile(LittleFS, "/hello0.txt");
  LittleFS.End();

  Serial.println( "Done with part2, work with the first lfs partition..." );
  #endif

  if(!LittleFS.Begin(FORMAT_LITTLEFS_IF_FAILED)){
    Serial.println("LittleFS Mount Failed");
    return;
  }

  Serial.println( "SPIFFS-like write file to new path and delete it w/folders" );
  writeFile_SPIFFS(LittleFS, "/new1/new2/new3/hello3.txt", "Hello3");
  showDirectories(LittleFS, "/", 3);

  deleteFile_SPIFFS(LittleFS, "/new1/new2/new3/hello3.txt");
  showDirectories(LittleFS, "/", 3);

  createDirectory(LittleFS, "/mydir");
  writeFile(LittleFS, "/mydir/hello2.txt", "Hello2");
  showDirectories(LittleFS, "/", 1);

  deleteFile(LittleFS, "/mydir/hello2.txt");
  removeDirectory(LittleFS, "/mydir");
  showDirectories(LittleFS, "/", 1);

  writeFile(LittleFS, "/hello.txt", "Hello ");
  appendFile(LittleFS, "/hello.txt", "World!\r\n");
  readFile(LittleFS, "/hello.txt");

  renameFile(LittleFS, "/hello.txt", "/foo.txt");
  readFile(LittleFS, "/foo.txt");
  deleteFile(LittleFS, "/foo.txt");

  benchIO(LittleFS, "/test.txt");
  deleteFile(LittleFS, "/test.txt");

  Serial.println("Test complete");
}

void loop() { }
