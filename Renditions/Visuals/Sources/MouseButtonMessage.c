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

#include "MouseButtonMessage.h"

#include "Shizu/Runtime/CxxUtilities.h"

static void
MouseButtonMessage_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const MouseButtonMessage_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(MouseButtonMessage),
  .construct = &MouseButtonMessage_constructImpl,
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(MouseButtonMessage_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.MouseButtonMessage", MouseButtonMessage, Shizu_Object);

static void
MouseButtonMessage_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (5 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Type* TYPE = MouseButtonMessage_getType(state);
  MouseButtonMessage* SELF = (MouseButtonMessage*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  SELF->action = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[1]);
  SELF->button = Shizu_Runtime_Extensions_getFloat32Value(state, &argumentValues[2]);
  SELF->x = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[3]);
  SELF->y = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[4]);
  ((Shizu_Object*)SELF)->type = TYPE;
}

MouseButtonMessage*
MouseButtonMessage_create
  (
    Shizu_State2* state,
    Shizu_Integer32 action,
    Shizu_Integer32 button,
    Shizu_Integer32 x,
    Shizu_Integer32 y
  )
{
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer(),
                                   Shizu_Value_Initializer(),
                                   Shizu_Value_Initializer(),
                                   Shizu_Value_Initializer(),
                                   Shizu_Value_Initializer(), };
  Shizu_Value_setType(&argumentValues[0], MouseButtonMessage_getType(state));
  Shizu_Value_setInteger32(&argumentValues[1], action);
  Shizu_Value_setInteger32(&argumentValues[2], button);
  Shizu_Value_setInteger32(&argumentValues[3], x);
  Shizu_Value_setInteger32(&argumentValues[4], y);
  Shizu_Operations_create(state, &returnValue, 5, &argumentValues[0]);
  return (MouseButtonMessage*)Shizu_Value_getObject(&returnValue);
}

Shizu_Integer32
MouseButtonMessage_getAction
  (
    Shizu_State2* state,
    MouseButtonMessage* self
  )
{
  Shizu_debugAssert(NULL != state);
  Shizu_debugAssert(NULL != self);
  return self->action;
}

Shizu_Integer32
MouseButtonMessage_getButton
  (
    Shizu_State2* state,
    MouseButtonMessage* self
  )
{
  Shizu_debugAssert(NULL != state);
  Shizu_debugAssert(NULL != self);
  return self->button;
}
