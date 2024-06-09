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

static void
doStartup
  (
    Shizu_State* state
  )
{ Visuals_ServiceGl_startup(state); }

static void
doShutdown
  (
    Shizu_State* state
  )
{ Visuals_ServiceGl_shutdown(state); }

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
{ Visuals_ServiceGl_setTitle(state, title); }

void
Visuals_Service_getClientSize
  (
    Shizu_State* state,
    Shizu_Integer32* width,
    Shizu_Integer32* height
  )
{ Visuals_ServiceGl_getClientSize(state, width, height); }

void
Visuals_Service_beginFrame
  (
    Shizu_State* state
  )
{ Visuals_ServiceGl_beginFrame(state); }

void
Visuals_Service_endFrame
  (
    Shizu_State* state
  )
{ Visuals_ServiceGl_endFrame(state); }

void
Visuals_Service_update
  (
    Shizu_State* state
  )
{ Visuals_ServiceGl_update(state); }

Shizu_Boolean
Visuals_Service_quitRequested
  (
    Shizu_State* state
  )
{ return Visuals_ServiceGl_quitRequested(state); }

void
Visuals_Service_emitKeyboardKeyMessage
  (
    Shizu_State* state,
    KeyboardKeyMessage* message
  )
{ Visuals_ServiceGl_emitKeyboardKeyMessage(state, message); }

void
Visuals_Service_addKeyboardKeyCallback
  (
    Shizu_State* state,
    Shizu_Value* value
  )
{ Visuals_ServiceGl_addKeyboardKeyCallback(state, value); }

void
Visuals_Service_registerVisualsObject
  (
    Shizu_State* state,
    Visuals_Object* object
  )
{ Visuals_ServiceGl_registerVisualsObject(state, object); }
