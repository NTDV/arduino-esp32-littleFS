# arduino-esp32-littleFS
Another Little File System implementation for esp32 core v2

# Before start
### You must create intial file system image before using the lib.
Do it with [special plugin](https://github.com/lorol/arduino-esp32fs-plugin) for Arduino IDE.
It can be used also for SPIFFS, FatFS not only for LittleFS.

# Install in Arduino IDE
1. Install [esp32 core v2](https://github.com/espressif/arduino-esp32)
2. Choose correct board in Arduino IDE
4. Download [latest release](https://github.com/NTDV/arduino-esp32-littleFS/releases/latest) as `.zip` archive
5. Unpack archive into `%USERPROFILE%\AppData\Local\Arduino15\packages\esp32\hardware\esp32\{version}\libraries\` on Windows
6. Restart Arduino IDE and enjoy the lib!

# Install in PlatformIO
1. Create or open existiong ESP32 project with arduino framework
2. Append `lib_deps = https://github.com/NTDV/arduino-esp32-littleFS.git` in your `platformio.ini` configuration
3. Reload the project and enjoy the lib!

# Using
Add `#include <LittleFileSystem.h>` in the beginning of file. Call special object `LittleFS` as you want.

There are some examples in `examples` folder. For Arduino IDE it can be found at `File -> Examples -> LittleFileSystem`.

Some docs avaliable [here](https://ntdv.github.io/arduino-esp32-littleFS/)
