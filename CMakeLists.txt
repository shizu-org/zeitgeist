# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(project_name "Zeitgeist")

project(${project_name})

include(FetchContent)

#######################################################################################################################

set(idlib-math.repository CACHE STRING "IdLib Math: The Git repository to check out")
if (NOT idlib-math.repository)
  set(idlib-math.repository https://github.com/michaelheilmann/idlib-math.git)
endif()

set(idlib-math.tag CACHE STRING "IdLib Math: The Git tag to check out")
if (NOT idlib-math.tag)
  set(idlib-math.tag 1.6)
endif()

set(idlib-math.source-dir CACHE STRING "IdLib Math: Path to the source directory")
if (NOT idlib-math.source-dir)
  set(idlib-math.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-math-1.6-src")
endif()

set(idlib-math.binary-dir CACHE STRING "IdLib Math: Path to the build directory")
if (NOT idlib-math.binary-dir)
  set(idlib-math.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-math-1.6-bld")
endif()

FetchContent_Declare(idlib-math
                     GIT_REPOSITORY ${idlib-math.repository}
                     GIT_TAG ${idlib-math.tag}
                     SOURCE_DIR ${idlib-math.source-dir}
                     BINARY_DIR ${idlib-math.binary-dir})

#######################################################################################################################

set(idlib-process.repository CACHE STRING "IdLib Process: The Git repository to check out")
if (NOT idlib-process.repository)
  set(idlib-process.repository https://github.com/michaelheilmann/idlib-process.git)
endif()

set(idlib-process.tag CACHE STRING "IdLib Process: The Git tag to check out")
if (NOT idlib-process.tag)
  set(idlib-process.tag 1.4)
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

set(idlib-bit-scan.repository CACHE STRING "IdLib Bit Scan: The Git repository to check out")
if (NOT idlib-bit-scan.repository)
  set(idlib-bit-scan.repository https://github.com/michaelheilmann/idlib-bit-scan.git)
endif()

set(idlib-bit-scan.tag CACHE STRING "IdLib Bit Scan: The Git tag to check out")
if (NOT idlib-bit-scan.tag)
  set(idlib-bit-scan.tag 1.4)
endif()

set(idlib-bit-scan.source-dir CACHE STRING "IdLib Bit Scan: Path to the source directory")
if (NOT idlib-bit-scan.source-dir)
  set(idlib-bit-scan.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-bit-scan-1.4-src")
endif()

set(idlib-bit-scan.binary-dir CACHE STRING "IdLib Bit Scan: Path to the build directory")
if (NOT idlib-bit-scan.binary-dir)
  set(idlib-bit-scan.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-bit-scan-1.4-bld")
endif()

#######################################################################################################################

set(idlib-file-system.repository CACHE STRING "IdLib File System: The Git repository to check out")
if (NOT idlib-file-systme.repository)
  set(idlib-file-system.repository https://github.com/michaelheilmann/idlib-file-system.git)
endif()

set(idlib-file-system.tag CACHE STRING "IdLib File System: The Git tag to check out")
if (NOT idlib-file-system.tag)
  set(idlib-file-system.tag 1.7)
endif()

set(idlib-fie-system.source-dir CACHE STRING "IdLib File System: Path to the source directory")
if (NOT idlib-file-system.source-dir)
  set(idlib-file-system.source-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-file-system-1.7-src")
endif()

set(idlib-file-system.binary-dir CACHE STRING "IdLib File System: Path to the build directory")
if (NOT idlib-file-system.binary-dir)
  set(idlib-file-system.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/idlib-file-system-1.7-bld")
endif()
                     
#######################################################################################################################

set(Shizu.Runtime.repository CACHE STRING "Shizu Runtime: The Git repository to check out")
if (NOT Shizu.Runtime.repository)
  set(Shizu.Runtime.repository https://github.com/michaelheilmann/shizu-runtime.git)
endif()

set(Shizu.Runtime.tag CACHE STRING "Shizu Runtime: The Git tag to check out")
if (NOT Shizu.Runtime.tag)
  set(Shizu.Runtime.tag develop)
endif()

set(Shizu.Runtime.source-dir CACHE STRING "Shizu Runtime: Path to the source directory")
if (NOT Shizu.Runtime.source-dir)
  set(Shizu.Runtime.source-dir "${CMAKE_CURRENT_BINARY_DIR}/shizu-runtime-develop-src")
endif()

set(Shizu.Runtime.binary-dir CACHE STRING "Shizu Runtime: Path to the build directory")
if (NOT Shizu.Runtime.binary-dir)
  set(Shizu.Runtime.binary-dir "${CMAKE_CURRENT_BINARY_DIR}/shizu-runtime-develop-bld")
endif()

FetchContent_Declare(Shizu.Runtime
                     GIT_REPOSITORY ${Shizu.Runtime.repository}
                     GIT_TAG ${Shizu.Runtime.tag}
                     SOURCE_DIR ${Shizu.Runtime.source-dir}
                     BINARY_DIR ${Shizu.Runtime.binary-dir})
                     
#######################################################################################################################

FetchContent_MakeAvailable(Shizu.Runtime idlib-math)

# Copy the files of the rendition to the interpreter binary directory.
# rendition_name The rendition name.
macro(copy_zeitgeist_plugin interpreter_name rendition_name)
  get_target_property(source_dir ${rendition_name} SOURCE_DIR)
  message(STATUS "copying ${rendition_name} to ${interpreter_name}")
  
  if (WIN32)
    set(suffix ".dll")
  else()
    set(suffix ".so")
  endif()

  add_custom_command(
    TARGET ${rendition_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${interpreter_name}>/Renditions/${rendition_name}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                                     # which executes "cmake - E copy_if_different..."
            ${source_dir}/README.txt                                                  # file to copy
            $<TARGET_FILE_DIR:${interpreter_name}>/Renditions/${rendition_name}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different                                     # which executes "cmake - E copy_if_different..."
            $<TARGET_FILE:${rendition_name}>                                          # file to copy
            $<TARGET_FILE_DIR:${project_name}-Interpreter>/Renditions/${rendition_name}/library${suffix}
  )
endmacro()

add_subdirectory(Library)
add_subdirectory(Interpreter)
add_subdirectory(Renditions/Hello-World)
add_subdirectory(Renditions/Hello-World-OpenGl)
add_subdirectory(Renditions/Room-OpenGl)
add_subdirectory(Renditions/OpenGl-Commons)
