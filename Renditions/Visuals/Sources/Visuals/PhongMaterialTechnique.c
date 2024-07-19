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

#include "Visuals/PhongMaterialTechnique.h"

static void
Visuals_PhongMaterialTechnique_materializeImpl
  (
    Shizu_State2* state,
    Visuals_PhongMaterialTechnique* self
  );

static void
Visuals_PhongMaterialTechnique_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_PhongMaterialTechnique* self
  );

static void
Visuals_PhongMaterialTechnique_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_PhongMaterialTechnique_Dispatch* self
  );

static void
Visuals_PhongMaterialTechnique_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Visuals_PhongMaterialTechnique_Type = {
  .preDestroyType = NULL,
  .postCreateType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_PhongMaterialTechnique),
  .construct = &Visuals_PhongMaterialTechnique_constructImpl,
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_PhongMaterialTechnique_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_PhongMaterialTechnique_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Visuals.PhongMaterialTechnique", Visuals_PhongMaterialTechnique, Visuals_MaterialTechnique);

static void
Visuals_PhongMaterialTechnique_materializeImpl
  (
    Shizu_State2* state,
    Visuals_PhongMaterialTechnique* self
  )
{ }

static void
Visuals_PhongMaterialTechnique_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_PhongMaterialTechnique* self
  )
{ }

static void
Visuals_PhongMaterialTechnique_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_PhongMaterialTechnique_Dispatch* self
  )
{
  ((Visuals_Object_Dispatch*)self)->materialize = (void(*)(Shizu_State2*, Visuals_Object*)) & Visuals_PhongMaterialTechnique_materializeImpl;
  ((Visuals_Object_Dispatch*)self)->unmaterialize = (void(*)(Shizu_State2*, Visuals_Object*)) & Visuals_PhongMaterialTechnique_unmaterializeImpl;
}

static void
Visuals_PhongMaterialTechnique_constructImpl
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
  Shizu_Type* TYPE = Visuals_PhongMaterialTechnique_getType(state);
  Visuals_PhongMaterialTechnique* SELF = (Visuals_PhongMaterialTechnique*)Shizu_Value_getObject(&argumentValues[0]);
  {
    Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
    Shizu_Value argumentValues[] = { Shizu_Value_InitializerObject(SELF) };
    Shizu_Type* PARENTTYPE = Shizu_Types_getParentType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), TYPE);
    Shizu_Type_getObjectTypeDescriptor(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), PARENTTYPE)->construct
    (state, &returnValue, 1, &argumentValues[0]);
  }

  SELF->ambientR = 85;
  SELF->ambientG = 85;
  SELF->ambientB = 85;

  SELF->diffuseR = 85;
  SELF->diffuseG = 85;
  SELF->diffuseB = 85;

  SELF->specularR = 85;
  SELF->specularG = 85;
  SELF->specularB = 85;

  SELF->shininess = 230;

  ((Shizu_Object*)SELF)->type = TYPE;
}

Visuals_PhongMaterialTechnique*
Visuals_PhongMaterialTechnique_create
  (
    Shizu_State2* state
  )
{
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerType(Visuals_PhongMaterialTechnique_getType(state)), };
  Shizu_Operations_create(state, &returnValue, 1, &argumentValues[0]);
  return (Visuals_PhongMaterialTechnique*)Shizu_Value_getObject(&returnValue);
}
