/*
  Shizu Runtime
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

#if !defined(SHIZU_RUNTIME_TYPE_PRIVATE_H_INCLUDED)
#define SHIZU_RUNTIME_TYPE_PRIVATE_H_INCLUDED

#if !defined(SHIZU_RUNTIME_PRIVATE) && 1 != SHIZU_RUNTIME_PRIVATE
  #error("Do not include `Shizu/Runtime/Type.private.h` directly. Include `Shizu/Runtime/Include.h` instead.")
#endif
#include "Shizu/Runtime/State.h"
#include "Shizu/Runtime/Gc.private.h"

// This flag is set after a successful call to the Shizu_OnStaticInitialize function of the type. If the call is not successful, the flag is not set.
// A successful call to Shizu_OnStaticUninitialize clears the flags.
#define Shizu_TypeFlags_StaticallyInitialized (1)

typedef struct Shizu_Type Shizu_Type;

struct Shizu_Type {
  Shizu_Type* next;
  Shizu_Type* parentType;
  uint8_t flags;
  struct {
    size_t hashValue;
    // The number of Bytes including the zero terminator.
    size_t numberOfBytes;
    // A pointer to an array of @a numberOfBytes containing a zero-terminated C string.
    char* bytes;
  } name;
  Shizu_OnTypeDestroyedCallback* typeDestroyed;
  Shizu_TypeDescriptor const* descriptor;
  // Capacity is the size of the array.
  // The first n <= capacity entries point to elements, the remaining elements are NULL. 
  struct {
    Shizu_Type** elements;
    size_t capacity;
  } children;
  // A pointer to the DLL from which the type originates from or the null pointer.
  Shizu_Dll* dll;
};

void
Shizu_Types_initialize
  (
    Shizu_State* self
  );

void
Shizu_Types_uninitialize
  (
    Shizu_State* self
) ;

#endif // SHIZU_RUNTIME_TYPE_PRIVATE_H_INCLUDED
