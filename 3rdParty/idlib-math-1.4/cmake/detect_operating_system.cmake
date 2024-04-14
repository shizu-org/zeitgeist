# Copyright (c) 2018-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

# Macro to define an enumeration of operating systems and detect the operating system.
#
# First, the enumeration constants ${target}.operating_system_(unknown|windows|unix|linux|macos|cygwin|msys|mingw|ios|ios_simulator) are defined.
# Each constant is a string of an unique name identifying an operating system.
#
# Second, the constant ${target}.operating_system is defined to the ${target}.operating_system_* value denoting the detected operating system.
#
# @param target The target.
#
# @remarks
# - "mingw" and/or "msys" is a "toolchain". The "operating system" is still "windows".
# - We are not aware of a reliable procedure to detect what Apple "operating system" is used.
#   This configuration script detects "Mac OS" even if the "operating system" is "iOS", "iOS Simulator", "watchOS", ... from within CMake?
macro(detect_operating_system target)
  set(${target}.operating_system_unknown "<unknown operating system>")

  set(${target}.operating_system_windows "WINDOWS")

  set(${target}.operating_system_unix "UNIX")

  set(${target}.operating_system_linux "LINUX")

  set(${target}.operating_system_macos "MACOS")

  set(${target}.operating_system_cygwin "CYGWIN")

  set(${target}.operating_system_msys "MSYS")

  set(${target}.operating_system_mingw "MINGW")

  set(${target}.operating_system_ios "IOS")

  set(${target}.operating_system_ios_simulator "IOS SIMULATOR")

  if (NOT DEFINED ${target}.operating_system_id)  
    set(${target}.operating_system ${${target}.operating_system_unknown})
    if (WIN32)
      set(${target}.operating_system ${${target}.operating_system_windows})
    elseif (CYGWIN)
      set(${target}.operating_system ${${target}.operating_system_cygwin}})  
    elseif (MSYS)
      set(${target}.operating_system ${${target}.operating_system_msys})
    endif()

    if (${${target}.operating_system} STREQUAL ${${target}.operating_system_unknown})
      if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
        set(${target}.operating_system ${${target}.operating_system_linux})
      elseif ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
        set(${target}.operating_system ${${target}.operating_system_macos})
      endif()
    endif()
  endif()
  message(STATUS " - ${target} operating system: ${${target}.operating_system}")
endmacro()
