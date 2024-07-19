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

#include "Vector4F32.h"

static void
Vector4F32_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  );

static Shizu_ObjectTypeDescriptor const Vector4F32_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Vector4F32),
  .construct = &Vector4F32_constructImpl,
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Vector4F32_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType("Zeitgeist.Vector4F32", Vector4F32, Shizu_Object);

static void
Vector4F32_constructImpl
  (
    Shizu_State2* state,
    Shizu_Value* returnValue,
    Shizu_Integer32 numberOfArgumentValues,
    Shizu_Value* argumentValues
  )
{
  if (5 != numberOfArgumentValues) {
    Shizu_State2_setStatus(state, Shizu_Status_NumberOfArgumentsInvalid);
    Shizu_State2_jump(state);
  }
  Shizu_Type* TYPE = Vector4F32_getType(state);
  Vector4F32* SELF = (Vector4F32*)Shizu_Value_getObject(&argumentValues[0]);
  Shizu_Object_construct(state, (Shizu_Object*)SELF);
  idlib_vector_4_f32_set(&SELF->v, Shizu_Runtime_Extensions_getFloat32Value(state, &argumentValues[1]),
                                   Shizu_Runtime_Extensions_getFloat32Value(state, &argumentValues[2]),
                                   Shizu_Runtime_Extensions_getFloat32Value(state, &argumentValues[3]),
                                   Shizu_Runtime_Extensions_getFloat32Value(state, &argumentValues[4]));
  ((Shizu_Object*)SELF)->type = TYPE;
}

Vector4F32*
Vector4F32_create
  (
    Shizu_State2* state,
    Shizu_Float32 x,
    Shizu_Float32 y,
    Shizu_Float32 z,
    Shizu_Float32 w
  )
{
  Shizu_Value returnValue = Shizu_Value_InitializerVoid(Shizu_Void_Void);
  Shizu_Value argumentValues[] = { Shizu_Value_InitializerType(Vector4F32_getType(state)),
                                   Shizu_Value_InitializerFloat32(x),
                                   Shizu_Value_InitializerFloat32(y),
                                   Shizu_Value_InitializerFloat32(z),
                                   Shizu_Value_InitializerFloat32(w) };
  Shizu_Operations_create(state, &returnValue, 5, &argumentValues[0]);
  return (Vector4F32*)Shizu_Value_getObject(&returnValue);
}

Vector4F32*
Vector4F32_add
  (
    Shizu_State2* state,
    Vector4F32* left,
    Vector4F32* right
  )
{
  Shizu_Type* type = Vector4F32_getType(state);
  Vector4F32* self = (Vector4F32*)Shizu_Gc_allocateObject(state, sizeof(Vector4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_vector_4_f32_add(&self->v, &left->v, &right->v);
  ((Shizu_Object*)self)->type = type;
  return self;
}

Vector4F32*
Vector4F32_subtract
  (
    Shizu_State2* state,
    Vector4F32* left,
    Vector4F32* right
  )
{
  Shizu_Type* type = Vector4F32_getType(state);
  Vector4F32* self = (Vector4F32*)Shizu_Gc_allocateObject(state, sizeof(Vector4F32));
  Shizu_Object_construct(state, (Shizu_Object*)self);
  idlib_vector_4_f32_subtract(&self->v, &left->v, &right->v);
  ((Shizu_Object*)self)->type = type;
  return self;
}
