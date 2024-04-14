# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/3rdParty/idlib-math-1.4/cmake/all.cmake)

set(name ${project_name}-Interpreter)
begin_executable()

list(APPEND ${name}.source_files Main.c)

end_executable()
target_link_libraries(${name} ${project_name}-Library)
