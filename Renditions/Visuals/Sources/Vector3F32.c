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

static void
Vector3F32_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Vector3F32_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Vector3F32),
  .construct = &Vector3F32_constructImpl,
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Vector3F32_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Vector3F32", Vector3F32, Shizu_Object);

static void
Vector3F32_constructImpl
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
  Shizu_Type* TYPE = Vector3F32_getType(state);
  Vector3F32* SELF = (Vector3F32*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  idlib_vector_3_f32_set(&SELF->v,  Shizu_Runtime_Extensions_getFloat32Value(state, &argumentValues[1]),
                                    Shizu_Runtime_Extensions_getFloat32Value(state, &argumentValues[2]),
                                    Shizu_Runtime_Extensions_getFloat32Value(state, &argumentValues[3]));
  ((Shizu_Object*)SELF)->type = TYPE;
}

Vector3F32*
Vector3F32_create
  (
    Shizu_State2* state,
    Shizu_Float32 x,
    Shizu_Float32 y,
    Shizu_Float32 z
  )
{
  Shizu_Value returnValue = Shizu_Value_Initializer();
  Shizu_Value argumentValues[] = { Shizu_Value_Initializer(),
                                   Shizu_Value_Initializer(),
                                   Shizu_Value_Initializer(),
                                   Shizu_Value_Initializer() };
  Shizu_Value_setType(&argumentValues[0], Vector3F32_getType(state));
  Shizu_Value_setFloat32(&argumentValues[1], x);
  Shizu_Value_setFloat32(&argumentValues[2], y);
  Shizu_Value_setFloat32(&argumentValues[3], z);
  Shizu_Operations_create(state, &returnValue, 4, &argumentValues[0]);
  return (Vector3F32*)Shizu_Value_getObject(&returnValue);
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
