# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${idlib-process.source-dir}/cmake/all.cmake)

set(name ${project_name}-Interpreter)
begin_executable()

list(APPEND ${name}.source_files Main.c)

end_executable()
target_link_libraries(${name} ${project_name}-Library idlib-process idlib-file-system)

# We must link libm under Linux.
if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_linux})
  target_link_libraries(${name} dl)
  target_compile_definitions(${name} PUBLIC _GNU_SOURCE)
endif()
