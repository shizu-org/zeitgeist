# Integrate with your CMake project
The proper way to integrate *IdLib Math* with your CMake project is by using `FetchContent`:

You need to fetch *IdLib Math* and its dependency *IdLib Process*:

You can do that as follows:

```
include(FetchContent)

#######################################################################################################################

set(idlib-process.repository CACHE STRING "IdLib Process: The Git repository to check out")
if (NOT idlib-process.repository)
  set(idlib-process.repository https://github.com/michaelheilmann/idlib-process.git)
endif()

set(idlib-process.tag CACHE STRING "IdLib Process: The Git tag to check out")
if (NOT idlib-process.tag)
  set(idlib-process.tag 1.2)
endif()

set(idlib-process.source-dir CACHE STRING "IdLib Process: Path to the source directory")
if (NOT idlib-process.source-dir)
  set(idlib-process.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-process-1.4-src")
endif()

set(idlib-process.binary-dir CACHE STRING "IdLib Process: Path to the build directory")
if (NOT idlib-process.binary-dir)
  set(idlib-process.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-process-1.4-bld")
endif()
                    
#######################################################################################################################

set(idlib-math.repository CACHE STRING "IdLib Math: The Git repository to check out")
if (NOT idlib-math.repository)
  set(idlib-math.repository https://github.com/michaelheilmann/idlib-math.git)
endif()

set(idlib-math.tag CACHE STRING "IdLib Math: The Git tag to check out")
if (NOT idlib-math.tag)
  set(idlib-math.tag 1.0)
endif()

set(idlib-math.source-dir CACHE STRING "IdLib Math: Path to the source directory")
if (NOT idlib-math.source-dir)
  set(idlib-math.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-math-1.0-src")
endif()

set(idlib-math.binary-dir CACHE STRING "IdLib Math: Path to the build directory")
if (NOT idlib-math.binary-dir)
  set(idlib-math.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-math-1.0-bld")
endif()

#######################################################################################################################

FetchContent_Declare(idlib-bit-scan
                     GIT_REPOSITORY ${idlib-math.repository}
                     GIT_TAG ${idlib-math.tag}
                     SOURCE_DIR ${idlib-math.source-dir}
                     BINARY_DIR ${idlib-math.binary-dir})

```

Now simply add
```
FetchContent_MakeAvailable(idlib-math)
```
And you can use *IdLib Math* by linking the library target `idlib-math` to a target `<target>`
```
target_link_libraries(<target> idlib-math)
```
and by adding the include directive  
```
#include "idlib/math.h"
```
in your C files.
