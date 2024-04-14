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

#if !defined(IDLIB_FILE_SYSTEM_ERRORS_H_INCLUDED)
#define IDLIB_FILE_SYSTEM_ERRORS_H_INCLUDED

/** 
 * @brief Symbolic constant indicating success.
 */
#define IDLIB_SUCCESS (0)

/**
 * @brief Symbolic constant indicating an unknown error.
 * The error was raised, however, the reason is not known.
 */
#define IDLIB_UNKNOWN_ERROR (1)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because a file was not found.
 */
#define IDLIB_FILE_NOT_FOUND (2)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because something was aborted.
 */
#define IDLIB_ABORTED (3)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because an argument was not valid.
 */
#define IDLIB_ARGUMENT_INVALID (4)

/** 
 * @brief Symbolic constant indicating an error.
 * The error was raised because an allocation failed.
 */
#define IDLIB_ALLOCATION_FAILED (5)

/** 
 * @brief Symbolic constant indicating an error.
 * The error was raised because something was too big.
 */
#define IDLIB_TOO_BIG (6)

/**
 * @brief Symbolic constant indicating an error.
 * The error was raised because a directory was not empty.
 */
#define IDLIB_DIRECTORY_NOT_EMPTY (7)

#endif // IDLIB_FILE_SYSTEM_ERRORS_H_INCLUDED
