# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${idlib-process.source-dir}/cmake/all.cmake)
include(${Shizu.source-dir}/cmake/all.cmake)

set(name ${project_name}-Interpreter)
Shizu_beginExecutable()

list(APPEND ${name}.source_files Main.c)

Shizu_endExecutable()
target_link_libraries(${name} ${project_name}-Library idlib-process)
add_dependencies(${name} Shizu.Modules.FileSystem)

get_target_property(prerequisiteModules ${name} prerequisiteModules)
list(APPEND prerequisiteModules Shizu.Modules.FileSystem)
list(APPEND prerequisiteModules Shizu.Modules.DataDefinitionLanguage)
list(APPEND prerequisiteModules Shizu.Modules.MachineLanguage)
set_target_properties(${name} PROPERTIES prerequisiteModules "${prerequisiteModules}")

on_executable(${name})

# We must link libm under Linux.
if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_linux})
  target_link_libraries(${name} dl)
  target_compile_definitions(${name} PUBLIC _GNU_SOURCE)
endif()

# Adjust the debugger working directory such that it is the output directory of the interpreters binary.
set_target_properties(${name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${name}>)
