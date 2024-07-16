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

#include "Visuals/BlinnPhongMaterial.h"

static void
Visuals_BlinnPhongMaterial_materializeImpl
  (
    Shizu_State2* state,
    Visuals_BlinnPhongMaterial* self
  );

static void
Visuals_BlinnPhongMaterial_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_BlinnPhongMaterial* self
  );
static void
Visuals_BlinnPhongMaterial_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_BlinnPhongMaterial_Dispatch* self
  );

static void
Visuals_BlinnPhongMaterial_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Visuals_BlinnPhongMaterial_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_BlinnPhongMaterial),
  .construct = &Visuals_BlinnPhongMaterial_constructImpl,
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_BlinnPhongMaterial_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_BlinnPhongMaterial_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Visuals.BlinnPhongMaterial", Visuals_BlinnPhongMaterial, Visuals_Material);

static void
Visuals_BlinnPhongMaterial_materializeImpl
  (
    Shizu_State2* state,
    Visuals_BlinnPhongMaterial* self
  )
{ }

static void
Visuals_BlinnPhongMaterial_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_BlinnPhongMaterial* self
  )
{ }

static void
Visuals_BlinnPhongMaterial_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_BlinnPhongMaterial_Dispatch* self
  )
{
  ((Visuals_Object_Dispatch*)self)->materialize = (void(*)(Shizu_State2*, Visuals_Object*)) & Visuals_BlinnPhongMaterial_materializeImpl;
  ((Visuals_Object_Dispatch*)self)->unmaterialize = (void(*)(Shizu_State2*, Visuals_Object*)) & Visuals_BlinnPhongMaterial_unmaterializeImpl;
}

static void
Visuals_BlinnPhongMaterial_constructImpl
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
  Shizu_Type* TYPE = Visuals_BlinnPhongMaterial_getType(state);
  Visuals_BlinnPhongMaterial* SELF = (Visuals_BlinnPhongMaterial*)Shizu_Value_getObject(&argumentValues[0]);
  Visuals_Material_construct(state, (Visuals_Material*)SELF);

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

void
Visuals_BlinnPhongMaterial_construct
  (
    Shizu_State2* state,
    Visuals_BlinnPhongMaterial* self
  )
{
  Shizu_Type* type = Visuals_BlinnPhongMaterial_getType(state);
  Visuals_Material_construct(state, (Visuals_Material*)self);

  self->ambientR = 85;
  self->ambientG = 85;
  self->ambientB = 85;

  self->diffuseR = 85;
  self->diffuseG = 85;
  self->diffuseB = 85;

  self->specularR = 85;
  self->specularG = 85;
  self->specularB = 85;

  self->shininess = 230;

  ((Shizu_Object*)self)->type = type;
}

Visuals_BlinnPhongMaterial*
Visuals_BlinnPhongMaterial_create
  (
    Shizu_State2* state
  )
{
  Visuals_BlinnPhongMaterial* self = (Visuals_BlinnPhongMaterial*)Shizu_Gc_allocateObject(state, sizeof(Visuals_BlinnPhongMaterial));
  Visuals_BlinnPhongMaterial_construct(state, self);
  return self;
}
