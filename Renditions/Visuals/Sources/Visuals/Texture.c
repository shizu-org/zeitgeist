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

#include "Visuals/Texture.h"

Shizu_defineEnumerationType("Zeitgeist.Visuals.PixelFormat", Visuals_PixelFormat);

static void
Visuals_Texture_finalize
  (
    Shizu_State2* state,
    Visuals_Texture* self
  );

static void
Visuals_Texture_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Texture_Dispatch* self
  );

static void
Visuals_Texture_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Visuals_Texture_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Texture),
  .construct = &Visuals_Texture_constructImpl,
  .finalize = (Shizu_OnFinalizeCallback*)&Visuals_Texture_finalize,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Texture_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_Texture_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Visuals.Texture", Visuals_Texture, Visuals_Object);

static void
Visuals_Texture_finalize
  (
    Shizu_State2* state,
    Visuals_Texture* self
  )
{/*Intentionally empty.*/}

static void
Visuals_Texture_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Texture_Dispatch* self
  )
{/*Intentionally empty.*/}

static void
Visuals_Texture_constructImpl
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
  Shizu_Type* TYPE = Visuals_Texture_getType(state);
  Visuals_Texture* SELF = (Visuals_Texture*)Shizu_Value_getObject(&argumentValues[0]);
  {
    Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
    Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF) };
    Shizu_Type* PARENTTYPE = Shizu_Types_getParentType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
    Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), PARENTTYPE)->construct
    (state, &returnValue, 1, &argumentValues[0]);
  }
  ((Shizu_Object*)SELF)->type = TYPE;
}
