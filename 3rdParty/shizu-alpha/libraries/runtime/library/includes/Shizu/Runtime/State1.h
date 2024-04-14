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

#if !defined(SHIZU_STATE1_H_INCLUDED)
#define SHIZU_STATE1_H_INCLUDED

#include "Shizu/Runtime/JumpTarget.h"

// bool
#include <stdbool.h>

/**
 * @since 1.0
 * The tier 1 state.
 * Provides
 * - get/set the error variable
 * - push pop on from the jump target stack, jumping to the top of the jump target stack
 * - get/set process exit flag
 */
typedef struct Shizu_State1 Shizu_State1;

/**
 * @since 1.0
 * @brief Acquire a refeference to the tier 1 state.
 * @param RETURN A pointer to a <code>Shizu_State1*</code> variable.
 * @return @a 0 on success. A non-zero value on failure.
 * @success The caller acquire a reference to the <code>Shizu_State1</code> value.
 * The address of the value was assigned th <code>*RETURN</code>.
 */
int
Shizu_State1_acquire
  (
    Shizu_State1** RETURN
  );

/**
 * @since 1.0
 * @brief Relinquish a refeference to the tier 1 state.
 * @param state A pointer to a <code>Shizu_State1</code> value.
 * @return @a 0 on success. A non-zero value on failure.
 * @success The caller relinquished his reference to the <code>Shizu_State1</code> value.
 */
int
Shizu_State1_relinquish
  (
    Shizu_State1* state
  );

/**
 * @since 1.0
 * @brief Push a jump target on the jump target stack of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @param jumpTarget A pointer to the jump target.
 */
void
Shizu_State1_pushJumpTarget
  (
    Shizu_State1* state,
    Shizu_JumpTarget* jumpTarget
  );

/**
 * @since 1.0
 * @brief Pop a jump target from the jump target stack of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @pre The jump target stack is not empty.
 */
void
Shizu_State1_popJumpTarget
  (
    Shizu_State1* state
  );

#if !defined(Shizu_NoReturn)

  #if Shizu_Configuration_CompilerC_Msvc == Shizu_Configuration_CompilerC

    #define Shizu_NoReturn() __declspec(noreturn)

  #else

    #define Shizu_NoReturn()

  #endif

#endif

/**
 * @since 1.0
 * @brief Jump to the jump target at the top of the jump target stack of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @pre The jump target stack is not empty.
 */
Shizu_NoReturn() void
Shizu_State1_jump
  (
    Shizu_State1* state
  );

/**
 * @since 1.0
 * @brief Set the error variable of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @param error The value to assign to the variable.
 */
void
Shizu_State1_setError
  (
    Shizu_State1* state,
    int error
  );

/**
 * @since 1.0
 * @brief Get the error variable of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @return The value assigned to the variable.
 */
int
Shizu_State1_getError
  (
    Shizu_State1* state
  );

/**
 * @since 1.0
 * @brief Set the process exit requested variable of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @param processExitRequested The value to assign to the variable.
 */
void
Shizu_State1_setProcessExitRequested
  (
    Shizu_State1* state,
    bool processExitRequested
  );

/**
 * @since 1.0
 * @brief Get the process exist requested variable of a tier 1 state.
 * @param state A pointer to the tier 1 state.
 * @return The value assigned to the variable.
 */
bool
Shizu_State1_getProcessExitRequested
  (
    Shizu_State1* state
  );

#endif // SHIZU_STATE1_H_INCLUDED