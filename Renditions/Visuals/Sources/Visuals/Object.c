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
Visuals_Object_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Object_Dispatch* self
  );

static void
Visuals_Object_notifyVisualsShutdownImpl
  (
    Shizu_State2* state,
    Visuals_Object* self
  );

static void
Visuals_Object_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Visuals_Object_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Object),
  .construct = &Visuals_Object_constructImpl,
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Object_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_Object_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Visuals.Object", Visuals_Object, Shizu_Object);

static void
Visuals_Object_dispatchInitialize
  (
    Shizu_State1* stat1e,
    Visuals_Object_Dispatch* self
  )
{
  self->notifyVisualsShutdown = &Visuals_Object_notifyVisualsShutdownImpl;
}

static void
Visuals_Object_notifyVisualsShutdownImpl
  (
    Shizu_State2* state,
    Visuals_Object* self
  )
{ Visuals_Object_unmaterialize(state, self); }

static void
Visuals_Object_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (1 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Type* TYPE = Visuals_Object_getType(state);
  Visuals_Object* SELF = (Visuals_Object*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  ((Shizu_Object*)SELF)->type = TYPE;
}

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
