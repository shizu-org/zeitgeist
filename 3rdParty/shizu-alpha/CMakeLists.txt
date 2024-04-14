# Copyright (c) 2023-2024 Michael Heilmann. All rights reserved.
# This file is licensed under the "zlib License".

cmake_minimum_required(VERSION 3.20)

# Set the project name.
project(Shizu LANGUAGES C)
set(Shizu.project-name "Shizu")

# Remember the path to the cmake directory.
set(Shizu.cmake-files-dir ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Include the cmake library.
include(${Shizu.cmake-files-dir}/all.cmake)

# Enable testing.
enable_testing()

# Add subdirectories.
add_subdirectory(libraries)
add_subdirectory(clients)
add_subdirectory(servers)
