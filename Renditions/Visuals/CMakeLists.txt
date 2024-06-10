# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${idlib-process.source-dir}/cmake/all.cmake)

set(name ${project_name}-Visuals)
begin_library()

list(APPEND ${name}.source_files Sources/Matrix4F32.c)
list(APPEND ${name}.header_files Sources/Matrix4F32.h)
list(APPEND ${name}.source_files Sources/Vector3F32.c)
list(APPEND ${name}.header_files Sources/Vector3F32.h)
list(APPEND ${name}.source_files Sources/Vector4F32.c)
list(APPEND ${name}.header_files Sources/Vector4F32.h)

list(APPEND ${name}.source_files Sources/KeyboardKeyMessage.c)
list(APPEND ${name}.header_files Sources/KeyboardKeyMessage.h)

list(APPEND ${name}.source_files Sources/Visuals/Object.c)
list(APPEND ${name}.header_files Sources/Visuals/Object.h)

list(APPEND ${name}.source_files Sources/Visuals/Texture.c)
list(APPEND ${name}.header_files Sources/Visuals/Texture.h)
list(APPEND ${name}.source_files Sources/Visuals/VertexBuffer.c)
list(APPEND ${name}.header_files Sources/Visuals/VertexBuffer.h)

end_library()

source_group(TREE ${CMAKE_CURRENT_BINARY_DIR} FILES ${${name}.configuration_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.header_files})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${${name}.source_files})

target_link_libraries(${name} ${project_name}-Library)

target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Sources)
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Sources)

target_link_libraries(${name} idlib-math)
