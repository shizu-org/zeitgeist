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

static Shizu_TypeDescriptor const Visuals_BlinnPhongMaterial_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_BlinnPhongMaterial),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_BlinnPhongMaterial_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Visuals_BlinnPhongMaterial, Visuals_Material);

void
Visuals_BlinnPhongMaterial_construct
  (
    Shizu_State* state,
    Visuals_BlinnPhongMaterial* self
  )
{
  Shizu_Type* type = Visuals_BlinnPhongMaterial_getType(state);
  Visuals_Material_construct(state, (Visuals_Material*)self);
  self->diffuse = 85;
  self->specular = 85;
  self->ambient = 85;
  self->shininess = 230;
  ((Shizu_Object*)self)->type = type;
}

Visuals_BlinnPhongMaterial*
Visuals_BlinnPhongMaterial_create
  (
    Shizu_State* state
  )
{
  Visuals_BlinnPhongMaterial* self = (Visuals_BlinnPhongMaterial*)Shizu_Gc_allocateObject(state, sizeof(Visuals_BlinnPhongMaterial));
  Visuals_BlinnPhongMaterial_construct(state, self);
  return self;
}