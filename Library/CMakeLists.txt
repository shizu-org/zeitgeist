# Copyright (c) 2024 Michael Heilmann. All rights reserved.

cmake_minimum_required(VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/3rdParty/idlib-math-1.4/cmake/all.cmake)

set(name ${project_name}-Library)
begin_library()

list(APPEND ${name}.source_files Zeitgeist/Configure.h.in)

list(APPEND ${name}.source_files Zeitgeist.c)
list(APPEND ${name}.header_files Zeitgeist.h)

list(APPEND ${name}.source_files Zeitgeist/Rendition.c)
list(APPEND ${name}.header_files Zeitgeist/Rendition.h)

list(APPEND ${name}.source_files Zeitgeist/UpstreamRequests.c)
list(APPEND ${name}.header_files Zeitgeist/UpstreamRequests.h)

# Configuration variables for 'Configure.h.in'.
if (${${name}.operating_system} STREQUAL ${${name}.operating_system_windows})
  set(Zeitgeist_Configuration_OperatingSystem Zeitgeist_Configuration_OperatingSystem_Windows)
endif()
if (${${name}.operating_system} STREQUAL ${${name}.operating_system_linux})
  set(Zeitgeist_Configuration_OperatingSystem Zeitgeist_Configuration_OperatingSystem_Linux)
endif()

if (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_msvc})
  set(Zeitgeist_Configuration_CompilerC Zeitgeist_Configuration_CompilerC_Msvc)
endif()
if (${${name}.compiler_c} STREQUAL ${${name}.compiler_c_gcc})
  set(Zeitgeist_Configuration_CompilerC Zeitgeist_Configuration_CompilerC_Gcc)
endif()

if (${${name}.instruction_set_architecture} STREQUAL ${${name}.instruction_set_architecture_x64})
  set(Zeitgeist_Configuration_InstructionSetArchitecture Zeitgeist_Configuration_InstructionSetArchitecture_X64)
endif()
if (${${name}.instruction_set_architecture} STREQUAL ${${name}.instruction_set_architecture_x86})
  set(Zeitgeist_Configuration_InstructionSetArchitecture Zeitgeist_Configuration_InstructionSetArchitecture_X86)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Zeitgeist/Configure.h.in Zeitgeist/Configure.h)
list(APPEND ${name}.source_files Zeitgeist/Configure.h)

end_library()

#target_link_libraries(${name})
target_link_libraries(${name} Shizu.Runtime)

target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
