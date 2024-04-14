# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

# Macro to define an enumeration of compilers and detect what compiler is used.
#
# First, the enumeration constants ${target}_compiler_c_(unknown|clang|msvc|gcc) are defined.
# Each constant is a string of an unique name identifying a C compiler.
#
# Second, the constant ${target}.compiler_c is set to the {target}.compiler_c_* values denoting the detected C compiler.
#
# @param target The target.
macro(detect_c_compiler target)
  # An unknown compiler.
  set(${target}.compiler_c_unknown "<unknown compiler>")

  # CLANG.
  set(${target}.compiler_c_clang "CLANG")

  # MSVC.
  set(${target}.compiler_c_msvc "MSVC")

  # GCC.
  set(${target}.compiler_c_gcc "GCC")

  # Initialize if not yet initialized.
  if (NOT DEFINED ${target_name}.compiler_c)
    set(${target}.compiler_c ${${target}.compiler_c_unknown})
  endif()
  # Perform detection.
  if (CMAKE_C_COMPILER_ID)
    if (CMAKE_C_COMPILER_ID MATCHES ".*clang")
      set(${target}.compiler_c ${${target}.compiler_c_clang})
    endif()
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set(${target}.compiler_c ${${target}.compiler_c_gcc})
    endif()
    if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
      set(${target}.compiler_c ${${target}.compiler_c_msvc})
    endif()
  endif()
  message( STATUS " - ${target} C compiler: ${${target}.compiler_c}")
endmacro()
