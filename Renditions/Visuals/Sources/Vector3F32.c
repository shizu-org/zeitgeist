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

#include "Vector3F32.h"

static Shizu_ObjectTypeDescriptor const Vector3F32_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Vector3F32),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Vector3F32_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Vector3F32, Shizu_Object);

Vector3F32*
Vector3F32_create
  (
    Shizu_State2* state,
    Shizu_Float32 x,
    Shizu_Float32 y,
    Shizu_Float32 z
  )
{
  Shizu_Type* type = Vector3F32_getType(state);
  Vector3F32* self = (Vector3F32*)Shizu_Gc_allocateObject(state, sizeof(Vector3F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_vector_3_f32_set(&self->v, x, y, z);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Vector3F32*
Vector3F32_add
  (
    Shizu_State2* state,
    Vector3F32* left,
    Vector3F32* right
  )
{
  Shizu_Type* type = Vector3F32_getType(state);
  Vector3F32* self = (Vector3F32*)Shizu_Gc_allocateObject(state, sizeof(Vector3F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_vector_3_f32_add(&self->v, &left->v, &right->v);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Vector3F32*
Vector3F32_subtract
  (
    Shizu_State2* state,
    Vector3F32* left,
    Vector3F32* right
  )
{
  Shizu_Type* type = Vector3F32_getType(state);
  Vector3F32* self = (Vector3F32*)Shizu_Gc_allocateObject(state, sizeof(Vector3F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_vector_3_f32_subtract(&self->v, &left->v, &right->v);
  ((Shizu_Object*)self)->type = type;
  return self;
}
