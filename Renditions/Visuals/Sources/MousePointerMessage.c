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

#include "MousePointerMessage.h"

#include "Shizu/Runtime/CxxUtilities.h"

static void
MousePointerMessage_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const MousePointerMessage_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(MousePointerMessage),
  .construct = &MousePointerMessage_constructImpl,
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(MousePointerMessage_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.MousePointerMessage", MousePointerMessage, Shizu_Object);

static void
MousePointerMessage_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (4 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Type* TYPE = MousePointerMessage_getType(state);
  MousePointerMessage* SELF = (MousePointerMessage*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  SELF->action = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[1]);
  SELF->x = Shizu_Runtime_Extensions_getFloat32Value(state, &argumentValues[2]);
  SELF->y = Shizu_Runtime_Extensions_getFloat32Value(state, &argumentValues[3]);
  ((Shizu_Object*)SELF)->type = TYPE;
}

MousePointerMessage*
MousePointerMessage_create
  (
    Shizu_State2* state,
    Shizu_Integer32 action,
    Shizu_Integer32 x,
    Shizu_Integer32 y
  )
{
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerType(MousePointerMessage_getType(state)),
                                   Shizu_Value_InitializerInteger32(action),
                                   Shizu_Value_InitializerInteger32(x),
                                   Shizu_Value_InitializerInteger32(y), };
  Shizu_Operations_create(state, &returnValue, 4, &argumentValues[0]);
  return (MousePointerMessage*)Shizu_Value_getObject(&returnValue);
}

Shizu_Integer32
MousePointerMessage_getAction
  (
    Shizu_State2* state,
    MousePointerMessage* self
  )
{
  Shizu_debugAssert(NULL != state);
  Shizu_debugAssert(NULL != self);
  return self->action;
}

Shizu_Integer32
MousePointerMessage_getX
  (
    Shizu_State2* state,
    MousePointerMessage* self
  )
{
  Shizu_debugAssert(NULL != state);
  Shizu_debugAssert(NULL != self);
  return self->x;
}

Shizu_Integer32
MousePointerMessage_getY
  (
    Shizu_State2* state,
    MousePointerMessage* self
  )
{
  Shizu_debugAssert(NULL != state);
  Shizu_debugAssert(NULL != self);
  return self->y;
}
