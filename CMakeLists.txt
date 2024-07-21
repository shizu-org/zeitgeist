# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

set(project_name "Zeitgeist")

project(${project_name})

include(FetchContent)

#######################################################################################################################               

set(Shizu.Runtime.repository CACHE STRING "Shizu Runtime: The Git repository to check out")
if (NOT Shizu.Runtime.repository)
  set(Shizu.Runtime.repository https://github.com/shizu-org/shizu-runtime.git)
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

FetchContent_MakeAvailable(Shizu.Runtime)                     
                     
#######################################################################################################################
set(idlib-process.source-dir CACHE STRING "IdLib Process: Path to the source directory")
if (NOT idlib-process.source-dir)
  set(idlib-process.source-dir "${Shizu.Runtime.source-dir}/Libraries/idlib-process")
endif()               

set(idlib-bit-scan.source-dir CACHE STRING "IdLib Bit Scan: Path to the source directory")
if (NOT idlib-bit-scan.source-dir)
  set(idlib-bit-scan.source-dir "${Shizu.Runtime.source-dir}/Libraries/idlib-bit-scan")
endif()

set(idlib-file-system.source-dir CACHE STRING "IdLib File System: Path to the source directory")
if (NOT idlib-file-system.source-dir)
  set(idlib-file-system.source-dir "${Shizu.Runtime.source-dir}/Libraries/idlib-file-system")
endif()   

#######################################################################################################################

set(idlib-math.source-dir CACHE STRING "IdLib Math: Path to the source directory")
if (NOT idlib-math.source-dir)
  set(idlib-math.source-dir "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/idlib-math")
endif()

add_subdirectory(${idlib-math.source-dir})
                     
#######################################################################################################################

 
# Copy the files of the rendition to the interpreter binary directory.
# rendition_name The rendition name.
macro(copy_zeitgeist_plugin interpreter_name rendition_name)
  get_target_property(source_dir ${rendition_name} SOURCE_DIR)
  message(STATUS " - ${rendition_name}: ${rendition_name} to ${interpreter_name}")
  
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

  # Get all assets relative to the asset directory.
  file(GLOB_RECURSE assets_to_copy RELATIVE "${source_dir}/Assets" "${source_dir}/Assets/*.*" )
  message(STATUS " - ${rendition_name} assets: ${assets_to_copy}")

  foreach (asset_to_copy ${assets_to_copy})
      add_custom_command(
        TARGET ${rendition_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${interpreter_name}>/Renditions/${rendition_name}/Assets
        COMMAND ${CMAKE_COMMAND} -E copy_if_different                                     # which executes "cmake - E copy_if_different..."
                ${source_dir}/Assets/${asset_to_copy}                                     # file to copy
                $<TARGET_FILE_DIR:${interpreter_name}>/Renditions/${rendition_name}/Assets/${asset_to_copy}
      )
  endforeach()

endmacro()

add_subdirectory(Library)
add_subdirectory(Interpreter)
add_subdirectory(Renditions/Demo-Hello-World)
add_subdirectory(Renditions/Demo-Quadriliteral)
add_subdirectory(Renditions/Room)
add_subdirectory(Renditions/Visuals)
add_subdirectory(Renditions/OpenGl-Commons)
