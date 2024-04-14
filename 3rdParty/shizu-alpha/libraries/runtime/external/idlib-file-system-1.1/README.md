# IdLib File System
*IdLib File System* provides cross-platform (Windows and Linux) file system functionality for embedding into C programs.
*IdLib File System* is licensed under the [zlib License](LICENSE).

## Integration into your CMake project
Integrating *IdLib File System* into the sources of your CMake-based project is the intended use of *IdLib File System*.

Follow these steps to integrate this library into your CMake-based project:

- Create a directory for external dependencies within your project directory. For example `3rdparty`.
- Create a folder `idlib-file-system` in that directory.
- Download/checkout *IdLib File System* into that folder such that the contents of the directory in which this `README.md` file is located in directly resides under `3rdparty/idlib-file-system`.
- Add to your project the `3rdparty/idlib-file-system/CMakeLists.txt` using `add_subdirectory`.
- Link your target `YourTarget` to *IdLib File System* using `target_link_libraries(YourTarget idlib-file-system)`.
- In your C/C++ code, you now can include now the functionality of *IdLib File System* by adding the preprocessor include directive `#include "idlib-file-system.h"`.

## Building
For development (or if you really want to use prebuilt binaries), you can do standalone builds of *IdLib File System*.

*IdLib File System* has been tested to build and run both under *Windows 11* and *Linux*.
Find the build instrutions for the respective systems below:
- [Building *IdLib File System*: Windows 11/Visual Studio Community](building-under-windows-11-visual-studio-community-20222)
- [Building *IdLib File System*: Linux](building-under-linux)
