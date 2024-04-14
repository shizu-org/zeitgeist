# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

# Macro to define an enumeration of instruction set architectures and detect the instruction set architecture.
#
# First, the enumeration constants ${target}.instruction_set_architecture_(x86|x64) are defined.
# Each constant is a string of an unique name identifying an instruction set architecture.
#
# Second, the constant ${target}.instruction_set_architecture is defined to the ${target}.instruction_set_architecture_* value denoting the detected instruction set architecture.
#
# @param target The target.
macro(detect_instruction_set_architecture target)
  # Unknown instruction set architecture.
  set(${target}.instruction_set_architecture_unknown "<unknown instruction set architecture>")

  # x86.
  set(${target}.instruction_set_architecture_x86 "x86")

  # x64.
  set(${target}.instruction_set_architecture_x64 "x64")

  # Initialize if not yet initialized.
  if (NOT DEFINED ${target_name}.compiler_c)
    set(${target}.instruction_set_architecture ${${target}.instruction_set_architecture_unknown})
  endif()
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(${target}.instruction_set_architecture ${${target}.instruction_set_architecture_x64})
  elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(${target}.instruction_set_architecture ${${target}.instruction_set_architecture_x86})
  endif()
  message( STATUS " - ${target} instruction set architecture: ${${target}.instruction_set_architecture}")
endmacro()
