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

#include "ColorRGBU8.h"

static void
ColorRGBU8_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const ColorRGBU8_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(ColorRGBU8),
  .construct = &ColorRGBU8_constructImpl,
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(ColorRGBU8_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.ColorRGBU8", ColorRGBU8, Shizu_Object);

static void
ColorRGBU8_constructImpl
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
  Shizu_Type* TYPE = ColorRGBU8_getType(state);
  ColorRGBU8* SELF = (ColorRGBU8*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  SELF->r = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[1]);
  SELF->g = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[2]);
  SELF->b = Shizu_Runtime_Extensions_getInteger32Value(state, &argumentValues[3]);
  ((Shizu_Object*)SELF)->type = TYPE;
}

ColorRGBU8*
ColorRGBU8_create
  (
    Shizu_State2* state,
    Shizu_Integer32 r,
    Shizu_Integer32 g,
    Shizu_Integer32 b
  )
{
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerType(ColorRGBU8_getType(state)),
                                   Shizu_Value_InitializerInteger32(r),
                                   Shizu_Value_InitializerInteger32(g),
                                   Shizu_Value_InitializerInteger32(b), };
  Shizu_Operations_create(state, &returnValue, 4, &argumentValues[0]);
  return (ColorRGBU8*)Shizu_Value_getObject(&returnValue);
}
