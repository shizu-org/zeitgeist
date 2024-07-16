# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${idlib-process.source-dir}/cmake/all.cmake)
include(${Shizu.Runtime.source-dir}/cmake/all.cmake)

set(name ${project_name}-Interpreter)
Shizu_beginExecutable()

list(APPEND ${name}.source_files Main.c)

Shizu_endExecutable()
target_link_libraries(${name} ${project_name}-Library idlib-process idlib-file-system)
add_dependencies(${name} Shizu.Runtime.Modules.FileSystem)

get_target_property(prerequisiteModules ${name} prerequisiteModules)
list(APPEND prerequisiteModules Shizu.Runtime.Modules.FileSystem)
list(APPEND prerequisiteModules Shizu.Runtime.Modules.DataDefinitionLanguage)
list(APPEND prerequisiteModules Shizu.Runtime.Modules.MachineLanguage)
set_target_properties(${name} PROPERTIES prerequisiteModules "${prerequisiteModules}")

#set(prerequisiteModules "")
#get_target_property(prerequisiteModules Shizu.Runtime prerequisiteModules)
#set_target_properties(${name} PROPERTIES prerequisiteModules ${prerequisiteModules})

on_executable(${name})

# We must link libm under Linux.
if (${${name}.operating_system_id} EQUAL ${${name}.operating_system_id_linux})
  target_link_libraries(${name} dl)
  target_compile_definitions(${name} PUBLIC _GNU_SOURCE)
endif()

# Adjust the debugger working directory such that it is the output directory of the interpreters binary.
set_target_properties(${name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${name}>)
