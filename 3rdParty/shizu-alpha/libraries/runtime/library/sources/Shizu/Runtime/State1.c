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

#include "Shizu/Runtime/State1.h"

#include "idlib/process.h"

// exit, EXIT_FAILURE
#include <stdlib.h>

// fprintf, stderr
#include <stdio.h>

// malloc, free
#include <malloc.h>

// strlen
#include <string.h>

struct Shizu_State1 {

  // The reference count of this tier 1 state.
  int referenceCount;

  // Reference to the process object.
  idlib_process* process;
  
  // Pointer to the top of the jump target stack (a singly linked list of jump targets).
  Shizu_JumpTarget* jumpTargets;

  // 0 if no error. A non-zero value otherwise.
  int error;
  
  // true if exit was requested. false otherwise.
  // initial value false.
  bool processExitRequested; 
  
};

#if Shizu_Configuration_CompilerC_Msvc == Shizu_Configuration_CompilerC

  #define Shizu_staticAssert(expression, message) \
    static_assert(expression, message);

#else

  #define Shizu_staticAssert(expression, message) \
    _Static_assert(expression, message);

#endif

#define Shizu_debugAssert(expression) \
  if (!(expression)) {\
    Shizu_debugAssertionFailed(__FILE__, __LINE__, #expression); \
  }

static void
Shizu_debugAssertionFailed
  (
    char const* file,
    int line,
    char const* expression
  );

static void
Shizu_debugAssertionFailed
  (
    char const* file,
    int line,
    char const* expression
  )
{
  fprintf(stderr, "%s:%d: debug assertion `%s` failed\n", file, line, expression);
  exit(EXIT_FAILURE);
}

#define NAME "<Shizu.State1>"

int
Shizu_State1_acquire
  (
    Shizu_State1** RETURN
  )
{
  if (!RETURN) {
    return 1;
  }
  idlib_process* process = NULL;
  if (idlib_acquire_process(&process)) {
    return 1;
  }

  Shizu_State1* self = NULL;
  int result = idlib_get_global(process, NAME, strlen(NAME), &self);
  if (result != IDLIB_SUCCESS && result != IDLIB_NOT_EXISTS) {
    idlib_relinquish_process(process);
    process = NULL;   
  }
  if (result == IDLIB_NOT_EXISTS) {
    self = malloc(sizeof(Shizu_State1));
    if (!self) {
      idlib_relinquish_process(process);
      process = NULL;
      return 1;
    }
    self->process = process;

    self->processExitRequested = false;
    self->error = 0;
    self->jumpTargets = NULL;

    if (idlib_add_global(self->process, NAME, strlen(NAME), self)) {
      free(self);
      self = NULL;
      idlib_relinquish_process(process);
      process = NULL;
      return 1;
    }    
  }
  self->referenceCount++;
  *RETURN = self;
  return 0;
}

int
Shizu_State1_relinquish
  (
    Shizu_State1* self
  )
{
  if (!self) {
    return 1;
  }
  if (0 == --self->referenceCount) {
    if (self->jumpTargets) {
      /* warn */
    }
    idlib_process* process = self->process;
    self->process = NULL;
    idlib_remove_global(process, NAME, strlen(NAME));
    idlib_relinquish_process(process);
    process = NULL;
    free(self);
    self = NULL;
  }
  return 0;
}

void
Shizu_State1_pushJumpTarget
  (
    Shizu_State1* self,
    Shizu_JumpTarget* jumpTarget
  )
{
  Shizu_debugAssert(NULL != jumpTarget);
  jumpTarget->previous = self->jumpTargets;
  self->jumpTargets = jumpTarget;
}

void
Shizu_State1_popJumpTarget
  (
    Shizu_State1* self
  )
{
  Shizu_debugAssert(NULL != self);
  self->jumpTargets = self->jumpTargets->previous;
}

Shizu_NoReturn() void
Shizu_State1_jump
  (
    Shizu_State1* self
  )
{
  Shizu_debugAssert(NULL != self);
  Shizu_debugAssert(NULL != self->jumpTargets);
  longjmp(self->jumpTargets->environment, -1);
}

void
Shizu_State1_setError
  (
    Shizu_State1* self,
    int error
  )
{
  Shizu_debugAssert(NULL != self);
  self->error = error;
}

int
Shizu_State1_getError
  (
    Shizu_State1* self
  )
{
  Shizu_debugAssert(NULL != self);
  return self->error;
}

void
Shizu_State1_setProcessExitRequested
  (
    Shizu_State1* self,
    bool processExitRequested
  )
{
  Shizu_debugAssert(NULL != self);
  self->processExitRequested = processExitRequested;
}

bool
Shizu_State1_getProcessExitRequested
  (
    Shizu_State1* self
  )
{
  Shizu_debugAssert(NULL != self);
  return self->processExitRequested;
}
