# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/3rdParty/idlib-math-1.4/cmake/all.cmake)

set(name ${project_name}-Room-OpenGl)
begin_plugin_library()

list(APPEND ${name}.source_files Sources/Main.c)
list(APPEND ${name}.source_files Sources/World.c)
list(APPEND ${name}.header_files Sources/World.h)

end_plugin_library()

target_link_libraries(${name} ${project_name}-OpenGl-Commons idlib-process ${project_name}-Library)
add_dependencies(${name} ${project_name}-Interpreter)

copy_plugin(${project_name}-Interpreter ${name})
