/*
  Zeitgeist
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

#include "Visuals/Service.package.h"

#include "Visuals/Gl/ServiceGl.h"

/// @brief The reference count of the "Visuals" service.
static Shizu_Integer32 g_referenceCount = 0;

/// List of Shizu_CxxFunction/Shizu_WeakReference values.
static Shizu_List* g_keyboardKeyListeners = NULL;
/// List of Shizu_CxxFunction/Shizu_WeakReference values.
static Shizu_List* g_mouseButtonListeners = NULL;
/// List of Shizu_CxxFunction/Shizu_WeakReference values.
static Shizu_List* g_mousePointerListeners = NULL;

static void
doStartup
  (
    Shizu_State* state
  )
{ Visuals_Gl_Service_startup(state); }

static void
doShutdown
  (
    Shizu_State* state
  )
{
  Visuals_Gl_Service_shutdown(state);
  if (g_mousePointerListeners) {
    Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_mousePointerListeners);
    g_mousePointerListeners = NULL;
  }
  if (g_mouseButtonListeners) {
    Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_mouseButtonListeners);
    g_mouseButtonListeners = NULL;
  }
  if (g_keyboardKeyListeners) {
    Shizu_Object_unlock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_keyboardKeyListeners);
    g_keyboardKeyListeners = NULL;
  }
}

void
Visuals_Service_startup
  (
    Shizu_State* state
  )
{
  if (Shizu_Integer32_Maximum == g_referenceCount) {
    Shizu_State_setStatus(state, Shizu_Status_OperationInvalid);
    Shizu_State_jump(state);
  }
  if (g_referenceCount == 0) {
    doStartup(state);
  }
  g_referenceCount++;
}

void
Visuals_Service_shutdown
  (
    Shizu_State* state
  )
{
  if (Shizu_Integer32_Minimum == g_referenceCount) {
    Shizu_State_setStatus(state, Shizu_Status_OperationInvalid);
    Shizu_State_jump(state);
  }
  if (g_referenceCount == 1) {
    doShutdown(state);
  }
  g_referenceCount--;
}

void
Visuals_Service_setTitle
  (
    Shizu_State* state,
    Shizu_String* title
  )
{ Visuals_Gl_Service_setTitle(state, title); }

void
Visuals_Service_getClientSize
  (
    Shizu_State* state,
    Shizu_Integer32* width,
    Shizu_Integer32* height
  )
{ Visuals_Gl_Service_getClientSize(state, width, height); }

void
Visuals_Service_beginFrame
  (
    Shizu_State* state
  )
{ Visuals_Gl_Service_beginFrame(state); }

void
Visuals_Service_endFrame
  (
    Shizu_State* state
  )
{ Visuals_Gl_Service_endFrame(state); }

void
Visuals_Service_update
  (
    Shizu_State* state
  )
{ Visuals_Gl_Service_update(state); }

Shizu_Boolean
Visuals_Service_quitRequested
  (
    Shizu_State* state
  )
{ return Visuals_Gl_Service_quitRequested(state); }

void
Visuals_Service_emitKeyboardKeyMessage
  (
    Shizu_State* state,
    KeyboardKeyMessage* message
  )
{
  if (!g_keyboardKeyListeners) {
    return;
  }
  for (size_t i = 0, n = Shizu_List_getSize(state, g_keyboardKeyListeners); i < n; ++i) {
    Shizu_Value temporary = Shizu_List_getValue(state, g_keyboardKeyListeners, i);
    if (Shizu_Value_isCxxFunction(&temporary)) {
      Shizu_CxxFunction* cxxFunction = Shizu_Value_getCxxFunction(&temporary);
      Shizu_Stack_pushObject(state, (Shizu_Object*)message);
      Shizu_Stack_pushInteger32(state, 1);
      (*cxxFunction)(state);
    } else {
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    }
  }
}

void
Visuals_Service_emitMouseButtonMessage
  (
    Shizu_State* state,
    MouseButtonMessage* message
  )
{
  if (!g_mouseButtonListeners) {
    return;
  }
  for (size_t i = 0, n = Shizu_List_getSize(state, g_mouseButtonListeners); i < n; ++i) {
    Shizu_Value temporary = Shizu_List_getValue(state, g_mouseButtonListeners, i);
    if (Shizu_Value_isCxxFunction(&temporary)) {
      Shizu_CxxFunction* cxxFunction = Shizu_Value_getCxxFunction(&temporary);
      Shizu_Stack_pushObject(state, (Shizu_Object*)message);
      Shizu_Stack_pushInteger32(state, 1);
      (*cxxFunction)(state);
    } else {
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    }
  }
}

void
Visuals_Service_emitMousePointerMessage
  (
    Shizu_State* state,
    MousePointerMessage* message
  )
{
  if (!g_mousePointerListeners) {
    return;
  }
  for (size_t i = 0, n = Shizu_List_getSize(state, g_mousePointerListeners); i < n; ++i) {
    Shizu_Value temporary = Shizu_List_getValue(state, g_mousePointerListeners, i);
    if (Shizu_Value_isCxxFunction(&temporary)) {
      Shizu_CxxFunction* cxxFunction = Shizu_Value_getCxxFunction(&temporary);
      Shizu_Stack_pushObject(state, (Shizu_Object*)message);
      Shizu_Stack_pushInteger32(state, 1);
      (*cxxFunction)(state);
    } else {
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      exit(EXIT_FAILURE);
    }
  }
}

void
Visuals_Service_addKeyboardKeyCallback
  (
    Shizu_State* state,
    Shizu_Value* value
  )
{
  if (!g_keyboardKeyListeners) {
    g_keyboardKeyListeners = Shizu_List_create(state);
    Shizu_JumpTarget jumpTarget;
    Shizu_State_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_Object_lock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_keyboardKeyListeners);
      Shizu_State_popJumpTarget(state);
    } else {
      Shizu_State_popJumpTarget(state);
      g_keyboardKeyListeners = NULL;
      Shizu_State_jump(state);
    }
  }
  if (Shizu_Value_isVoid(value)) {
    return;
  } else if (Shizu_Value_isCxxFunction(value)) {
    Shizu_List_appendValue(state, g_keyboardKeyListeners, value);
  } else if (Shizu_Value_isObject(value)) {
    Shizu_Value temporary;
    Shizu_Value_setObject(&temporary, (Shizu_Object*)Shizu_WeakReference_create(state, (Shizu_Object*)Shizu_Value_getObject(value)));
    Shizu_List_appendValue(state, g_keyboardKeyListeners, &temporary);
  } else {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
}

void
Visuals_Service_addMouseButtonCallback
  (
    Shizu_State* state,
    Shizu_Value* value
  )
{
  if (!g_mouseButtonListeners) {
    g_mouseButtonListeners = Shizu_List_create(state);
    Shizu_JumpTarget jumpTarget;
    Shizu_State_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_Object_lock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_mouseButtonListeners);
      Shizu_State_popJumpTarget(state);
    } else {
      Shizu_State_popJumpTarget(state);
      g_mouseButtonListeners = NULL;
      Shizu_State_jump(state);
    }
  }
  if (Shizu_Value_isVoid(value)) {
    return;
  } else if (Shizu_Value_isCxxFunction(value)) {
    Shizu_List_appendValue(state, g_mouseButtonListeners, value);
  } else if (Shizu_Value_isObject(value)) {
    Shizu_Value temporary;
    Shizu_Value_setObject(&temporary, (Shizu_Object*)Shizu_WeakReference_create(state, (Shizu_Object*)Shizu_Value_getObject(value)));
    Shizu_List_appendValue(state, g_mouseButtonListeners, &temporary);
  } else {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
}

void
Visuals_Service_addMousePointerCallback
  (
    Shizu_State* state,
    Shizu_Value* value
  )
{
  if (!g_mousePointerListeners) {
    g_mousePointerListeners = Shizu_List_create(state);
    Shizu_JumpTarget jumpTarget;
    Shizu_State_pushJumpTarget(state, &jumpTarget);
    if (!setjmp(jumpTarget.environment)) {
      Shizu_Object_lock(Shizu_State_getState1(state), Shizu_State_getLocks(state), (Shizu_Object*)g_mousePointerListeners);
      Shizu_State_popJumpTarget(state);
    } else {
      Shizu_State_popJumpTarget(state);
      g_mousePointerListeners = NULL;
      Shizu_State_jump(state);
    }
  }
  if (Shizu_Value_isVoid(value)) {
    return;
  } else if (Shizu_Value_isCxxFunction(value)) {
    Shizu_List_appendValue(state, g_mousePointerListeners, value);
  } else if (Shizu_Value_isObject(value)) {
    Shizu_Value temporary;
    Shizu_Value_setObject(&temporary, (Shizu_Object*)Shizu_WeakReference_create(state, (Shizu_Object*)Shizu_Value_getObject(value)));
    Shizu_List_appendValue(state, g_mousePointerListeners, &temporary);
  } else {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
}

void
Visuals_Service_registerVisualsObject
  (
    Shizu_State* state,
    Visuals_Object* object
  )
{ Visuals_Gl_Service_registerVisualsObject(state, object); }


Shizu_String*
Visuals_Service_getBackendVendorName
  (
    Shizu_State* state
  )
{ return Visuals_Gl_Service_getBackendVendorName(state); }

Shizu_String*
Visuals_Service_getBackendRendererName
  (
    Shizu_State* state
  )
{ return Visuals_Gl_Service_getBackendRendererName(state); }

Shizu_Integer32
Visuals_Service_getBackendMajorVersion
  (
    Shizu_State* state
  )
{ return Visuals_Gl_Service_getBackendMajorVersion(state); }

Shizu_Integer32
Visuals_Service_getBackendMinorVersion
(
  Shizu_State* state
)   {
 return Visuals_Gl_Service_getBackendMinorVersion(state); }
