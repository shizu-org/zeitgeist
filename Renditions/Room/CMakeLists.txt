# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${idlib-process.source-dir}/cmake/all.cmake)

set(name ${project_name}-Room)
begin_plugin_library()

list(APPEND ${name}.source_files Sources/Main.c)
list(APPEND ${name}.source_files Sources/World.c)
list(APPEND ${name}.header_files Sources/World.h)
list(APPEND ${name}.source_files Sources/Player.c)
list(APPEND ${name}.header_files Sources/Player.h)
list(APPEND ${name}.header_files Sources/Loader.h)

end_plugin_library()

target_link_libraries(${name} ${project_name}-OpenGl-Commons idlib-process ${project_name}-Library)
add_dependencies(${name} ${project_name}-Interpreter)

copy_zeitgeist_plugin(${project_name}-Interpreter ${name})
