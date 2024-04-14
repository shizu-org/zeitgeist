/*
  IdLib File System
  Copyright (C) 2024 Michael Heilmann. All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#if !defined(IDLIB_ENUMERATE_FILES_H_INCLUDED)
#define IDLIB_ENUMERATE_FILES_H_INCLUDED

#include "configure.h"

// bool
#include <stdbool.h>

// size_t
#include <stddef.h>

typedef bool (idlib_enumerate_files_callback)(void *context, void const* bytes, size_t number_of_bytes);

/**
 * @brief Enumerate the files in a directory file.
 * @param path_name The pathname of the directory file.
 * @param callback The callback to invoke.
 * @remarks The file denoted by @a path_name must exist and must be a directory file.
 * @return #IDLIB_SUCCESS on success. A non-zero value on failure.
 */
int
idlib_enumerate_files
  (
    char const* path_name,
    void* context,
    idlib_enumerate_files_callback* callback,
    bool regular_files,
    bool directory_files
  );

#endif // IDLIB_ENUMERATE_FILES_H_INCLUDED
