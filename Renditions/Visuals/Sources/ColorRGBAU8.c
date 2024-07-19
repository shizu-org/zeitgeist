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

#include "ColorRGBAU8.h"

static void
ColorRGBAU8_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const ColorRGBAU8_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(ColorRGBAU8),
  .construct = &ColorRGBAU8_constructImpl,
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(ColorRGBAU8_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.ColorRGBAU8", ColorRGBAU8, Shizu_Object);

static void
ColorRGBAU8_constructImpl
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
  Shizu_Type* TYPE = ColorRGBAU8_getType(state);
  ColorRGBAU8* SELF = (ColorRGBAU8*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  SELF->r = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[1]);
  SELF->g = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[2]);
  SELF->b = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[3]);
  SELF->a = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[4]);
  ((Shizu_Object*)SELF)->type = TYPE;
}

ColorRGBAU8*
ColorRGBAU8_create
  (
    Shizu_State2* state,
    Shizu_Integer32 r,
    Shizu_Integer32 g,
    Shizu_Integer32 b,
    Shizu_Integer32 a
  )
{
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerType(ColorRGBAU8_getType(state)),
                                   Shizu_Value_InitializerInteger32(r),
                                   Shizu_Value_InitializerInteger32(g),
                                   Shizu_Value_InitializerInteger32(b),
                                   Shizu_Value_InitializerInteger32(a), };
  Shizu_Operations_create(state, &returnValue, 5, &argumentValues[0]);
  return (ColorRGBAU8*)Shizu_Value_getObject(&returnValue);
}
