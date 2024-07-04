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

#include "Visuals/PhongMaterial.h"

static void
Visuals_PhongMaterial_materializeImpl
  (
    Shizu_State2* state,
    Visuals_PhongMaterial* self
  );

static void
Visuals_PhongMaterial_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_PhongMaterial* self
  );

static void
Visuals_PhongMaterial_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_PhongMaterial_Dispatch* self
  );

static Shizu_ObjectTypeDescriptor const Visuals_PhongMaterial_Type = {
  .preDestroyType = NULL,
  .postCreateType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_PhongMaterial),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_PhongMaterial_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_PhongMaterial_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Visuals_PhongMaterial, Visuals_Material);

static void
Visuals_PhongMaterial_materializeImpl
  (
    Shizu_State2* state,
    Visuals_PhongMaterial* self
  )
{ }

static void
Visuals_PhongMaterial_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_PhongMaterial* self
  )
{ }

static void
Visuals_PhongMaterial_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_PhongMaterial_Dispatch* self
  )
{
  ((Visuals_Object_Dispatch*)self)->materialize = (void(*)(Shizu_State2*, Visuals_Object*)) & Visuals_PhongMaterial_materializeImpl;
  ((Visuals_Object_Dispatch*)self)->unmaterialize = (void(*)(Shizu_State2*, Visuals_Object*)) & Visuals_PhongMaterial_unmaterializeImpl;
}

void
Visuals_PhongMaterial_construct
  (
    Shizu_State2* state,
    Visuals_PhongMaterial* self
  )
{
  Shizu_Type* type = Visuals_PhongMaterial_getType(state);
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

Visuals_PhongMaterial*
Visuals_PhongMaterial_create
  (
    Shizu_State2* state
  )
{
  Visuals_PhongMaterial* self = (Visuals_PhongMaterial*)Shizu_Gc_allocateObject(state, sizeof(Visuals_PhongMaterial));
  Visuals_PhongMaterial_construct(state, self);
  return self;
}
