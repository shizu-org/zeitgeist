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

static Shizu_ObjectTypeDescriptor const Visuals_Material_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Material),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Material_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Visuals_Material, Visuals_Object);

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
