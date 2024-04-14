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

#if !defined(SHIZU_RUNTIME_STATE_H_INCLUDED)
#define SHIZU_RUNTIME_STATE_H_INCLUDED




// setjmp, jmp_buf, longjmp
#include <setjmp.h>

// bool
#include <stdbool.h>

//
#include "Shizu/Runtime/Configure.h"

//
#include "Shizu/Runtime/Type.h"




#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>

  #define Shizu_OperatingSystem_DllExtension ".dll"
  #define Shizu_OperatingSystem_DirectorySeparator "\\"

  typedef HMODULE Shizu_OperatingSystem_DllHandle;
  #define Shizu_OperatingSystem_DllHandle_Invalid (NULL)

#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem

  #include <dlfcn.h>

  #define Shizu_OperatingSystem_DllExtension ".so"
  #define Shizu_OperatingSystem_DirectorySeparator "/"

  typedef void* Shizu_OperatingSystem_DllHandle;
  #define Shizu_OperatingSystem_DllHandle_Invalid (NULL)

#else

  #error("operating system not (yet) supported")

#endif

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  static inline Shizu_OperatingSystem_DllHandle
  Shizu_OperatingSystem_loadDll
    (
      char const* path
    )
  { return LoadLibrary(path); }

#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem

  static inline Shizu_OperatingSystem_DllHandle
  Shizu_OperatingSystem_loadDll
    (
      char const* path
    )
  { return dlopen(path); }

#else

  #error("operating system not (yet) supported")

#endif

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  static inline void
  Shizu_OperatingSystem_unloadDll
    (
      Shizu_OperatingSystem_DllHandle handle
    )
  {
    if (Shizu_OperatingSystem_DllHandle_Invalid != handle) {
      FreeModule(handle);
    }
  }

#elif Zeitgeist_Configuration_OperatingSystem_Linux == Zeitgeist_Configuration_OperatingSystem

  static inline void
  Shizu_OperatingSystem_unloadDll
    (
      Shizu_OperatingSystem_DllHandle handle
    )
  {
    if (Shizu_OperatingSystem_DllHandle_Invalid != handle) {
      dlclose(handle);
    }
  }

#else

  #error("operating system not (yet) supported")

#endif

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  static inline void*
  Shizu_OperatingSystem_getDllSymbol
    (
      Shizu_OperatingSystem_DllHandle libraryHandle,
      char const* symbolName
    )
  { return GetProcAddress(libraryHandle, symbolName); }

#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem

  static inline void*
  Shizu_OperatingSystem_getDllSymbol
    (
      Shizu_OperatingSystem_DllHandle libraryHandle,
      char const* symbolName
    )
  { return dlsym(libraryHandle, symbolName); }

#else

  #error("operating system not (yet) supported")

#endif




#if Shizu_Configuration_Compiler_Msvc == Shizu_Configuration_Compiler

  #define Shizu_staticAssert(expression, message) \
    static_assert(expression, message);

#else

  #define Shizu_staticAssert(expression, message) \
    static_assert(expression, message);

#endif

#define Shizu_debugAssert(expression) \
  if (!(expression)) {\
    Shizu_debugAssertionFailed(__FILE__, __LINE__, #expression); \
  }

void
Shizu_debugAssertionFailed
  (
    char const* file,
    int line,
    char const* expression
  );

typedef struct Shizu_Object Shizu_Object;

typedef struct Shizu_State Shizu_State;

typedef struct Shizu_Gc Shizu_Gc;

typedef struct Shizu_Locks Shizu_Locks;

typedef struct Shizu_Stack Shizu_Stack;




typedef struct Shizu_JumpTarget Shizu_JumpTarget;

struct Shizu_JumpTarget {
  Shizu_JumpTarget* previous;
  jmp_buf environment;
};

int
Shizu_State_create
  (
    Shizu_State** RETURN
  );

int
Shizu_State_destroy
  (
    Shizu_State* self
  );

void
Shizu_State_pushJumpTarget
  (
    Shizu_State* self,
    Shizu_JumpTarget* jumpTarget
  );

void
Shizu_State_popJumpTarget
  (
    Shizu_State* self
  );

void
Shizu_State_setError
  (
    Shizu_State* self,
    int error
  );

int
Shizu_State_getError
  (
    Shizu_State* self
  );

#if Shizu_Configuration_CompilerC_Msvc == Shizu_Configuration_CompilerC
  #define Shizu_NoReturn() __declspec(noreturn)
#else
  #define Shizu_NoReturn()
#endif

Shizu_NoReturn() void
Shizu_State_jump
  (
    Shizu_State* self
  );

void
Shizu_State_setProcessExitRequested
  (
    Shizu_State* state,
    bool processExitRequested
  );

bool
Shizu_State_getProcessExitRequested
  (
    Shizu_State* state
  );



Shizu_Gc*
Shizu_State_getGc
  (
    Shizu_State* self
  );

Shizu_Locks*
Shizu_State_getLocks
  (
    Shizu_State* self
  );

Shizu_Stack*
Shizu_State_getStack
  (
    Shizu_State* self
  );

/// @brief Allocate named memory.
/// @param name The name.
/// @param n The size.
/// @error Named memory of the specified name already exists.
/// @error An allocation failed.
/// @error name is a null pointer.
/// @return A zero value on success. A non-zero value on failure.
int
Shizu_State_allocateNamedMemory
  (
    Shizu_State* state,
    char const* name,
    size_t n
  );

/// @brief Deallocate named memory.
/// @param name The name of the named memory.
int
Shizu_State_deallocateNamedMemory
  (
    Shizu_State* state,
    char const* name
  );

/// @brief Get type-scoped memory.
/// @param name
/// @error no memory of the specified ID exists.
int
Shizu_State_getNamedMemory
  (
    Shizu_State* state,
    char const* name,
    void** p
  );

typedef struct Shizu_Dll Shizu_Dll;

Shizu_Dll*
Shizu_State_getOrLoadDll
  (
    Shizu_State* state,
    char const* pathName,
    bool loadDll
  );

Shizu_Dll*
Shizu_State_getDlByName
  (
    Shizu_State* state,
    char const* name
  );

Shizu_Dll*
Shizu_State_getDlByAdr
(
  Shizu_State* state,
  void* p
);

void
Shizu_Dll_ref
  (
    Shizu_State* state,
    Shizu_Dll* dll
  );

void
Shizu_Dll_unref
  (
    Shizu_State* state,
    Shizu_Dll* dll
  );

void*
Shizu_Dll_getSymbol
  (
    Shizu_State* state,
    Shizu_Dll* dll,
    char const* symbolName
  );

#endif // SHIZU_RUNTIME_STATE_H_INCLUDED
