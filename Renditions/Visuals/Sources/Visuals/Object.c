/*
  Shizu Visuals
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

#include "Visuals/Object.h"

static void
dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Object_Dispatch* self
  );

static void
notifyVisualsShutdownImpl
  (
    Shizu_State2* state,
    Visuals_Object* self
  );

static Shizu_TypeDescriptor const Visuals_Object_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Object),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Object_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Visuals_Object, Shizu_Object);

static void
dispatchInitialize
  (
    Shizu_State1* stat1e,
    Visuals_Object_Dispatch* self
  )
{
  self->notifyVisualsShutdown = &notifyVisualsShutdownImpl;
}

static void
notifyVisualsShutdownImpl
  (
    Shizu_State2* state,
    Visuals_Object* self
  )
{ Visuals_Object_unmaterialize(state, self); }

void
Visuals_Object_construct
  (
    Shizu_State2* state,
    Visuals_Object* self
  )
{
  Shizu_Type* type = Visuals_Object_getType(state);
  Shizu_Object_construct(state, (Shizu_Object*)self);
  ((Shizu_Object*)self)->type = type;
}
