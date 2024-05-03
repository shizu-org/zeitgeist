# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${idlib-process.source-dir}/cmake/all.cmake)

set(name ${project_name}-Hello-World)
begin_plugin_library()

list(APPEND ${name}.source_files Sources/Main.c)

end_plugin_library()

target_link_libraries(${name} ${project_name}-Library)
add_dependencies(${name} ${project_name}-Interpreter)

copy_zeitgeist_plugin(${project_name}-Interpreter ${name})
