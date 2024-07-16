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

#include "Visuals/Material.h"

static void
Visuals_Material_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Visuals_Material_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Material),
  .construct = &Visuals_Material_constructImpl,
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Material_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Visuals.Material", Visuals_Material, Visuals_Object);

static void
Visuals_Material_constructImpl
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
  Shizu_Type* TYPE = Visuals_Material_getType(state);
  Visuals_Material* SELF = (Visuals_Material*)Shizu_Value_getObject(&argumentValues[0]);
  Visuals_Object_construct(state, (Visuals_Object*)SELF);
  ((Shizu_Object*)SELF)->type = TYPE;
}

void
Visuals_Material_construct
  (
    Shizu_State2* state,
    Visuals_Material* self
  )
{
  Shizu_Type* type = Visuals_Material_getType(state);
  Visuals_Object_construct(state, (Visuals_Object*)self);
  ((Shizu_Object*)self)->type = type;
}
