# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/3rdParty/idlib-math-1.4/cmake/all.cmake)

set(name ${project_name}-OpenGl-Commons)
begin_library()

if (WIN32)
  list(APPEND ${name}.source_files Sources/ServiceWgl.c)
  list(APPEND ${name}.header_files Sources/ServiceWgl.h)
else()
  find_package(X11 REQUIRED)
  find_package(OpenGL REQUIRED)
  if (OpenGL_GLX_FOUND)
    list(APPEND ${name}.source_files Sources/ServiceGlx.c)
    list(APPEND ${name}.header_files Sources/ServiceGlx.h)
  else()
    message(FATAL_ERROR "unble to find GLX")
  endif()
endif()

list(APPEND ${name}.source_files Sources/ServiceGl.c)
list(APPEND ${name}.header_files Sources/ServiceGl.h)

list(APPEND ${name}.source_files Sources/ServiceGl_Functions.i)

list(APPEND ${name}.source_files Sources/Matrix4R32.c)
list(APPEND ${name}.header_files Sources/Matrix4R32.h)

list(APPEND ${name}.source_files Sources/Vector3R32.c)
list(APPEND ${name}.header_files Sources/Vector3R32.h)

list(APPEND ${name}.source_files Sources/KeyboardKeyMessage.c)
list(APPEND ${name}.header_files Sources/KeyboardKeyMessage.h)

list(APPEND ${name}.source_files Sources/RenderBuffer.c)
list(APPEND ${name}.header_files Sources/RenderBuffer.h)

list(APPEND ${name}.source_files Sources/Visuals/Program.c)
list(APPEND ${name}.header_files Sources/Visuals/Program.h)
list(APPEND ${name}.source_files Sources/Visuals/Object.c)
list(APPEND ${name}.header_files Sources/Visuals/Object.h)

end_library()

source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${${name}.configuration_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.header_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.source_files})

target_link_libraries(${name} ${project_name}-Library)

target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Sources)
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Sources)

if (WIN32)
  target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/External/OpenGl/Includes)
else()
  set_property(TARGET ${name} PROPERTY POSITION_INDEPENDENT_CODE ON)
  target_link_libraries(${name} OpenGL::GLX ${X11_LIBRARIES})
endif()
target_link_libraries(${name} idlib-math)
