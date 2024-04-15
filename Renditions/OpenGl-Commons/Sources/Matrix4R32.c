#include "Matrix4R32.h"

#include "Vector3R32.h"

#if Zeitgeist_Configuration_CompilerC_Msvc == Zeitgeist_Configuration_CompilerC

  #pragma push_macro("near")
  #undef near

  #pragma push_macro("far")
  #undef far

#endif

Shizu_TypeDescriptor const Matrix4R32_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .size = sizeof(Matrix4R32),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Matrix4R32_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Matrix4R32, Shizu_Object);

Matrix4R32*
Matrix4R32_createIdentity
  (
    Shizu_State* state
  )
{
  Matrix4R32* self = (Matrix4R32*)Shizu_Gc_allocate(state, sizeof(Matrix4R32));
  idlib_matrix_4x4_f32_set_identity(&self->m);
  ((Shizu_Object*)self)->type = Matrix4R32_getType(state);
  return self;
}

Matrix4R32*
Matrix4R32_createTranslate
  (
    Shizu_State* state,
    Vector3R32* translate
  )
{
  Matrix4R32* self = (Matrix4R32*)Shizu_Gc_allocate(state, sizeof(Matrix4R32));
  idlib_matrix_4x4_f32_set_translate(&self->m, &translate->v);
  ((Shizu_Object*)self)->type = Matrix4R32_getType(state);
  return self;
}

Matrix4R32*
Matrix4R32_createRotateY
  (
    Shizu_State* state,
    Shizu_Float32 degrees
  )
{
  Matrix4R32* self = (Matrix4R32*)Shizu_Gc_allocate(state, sizeof(Matrix4R32));
  idlib_matrix_4x4_f32_set_rotation_y(&self->m, degrees);
  ((Shizu_Object*)self)->type = Matrix4R32_getType(state);
  return self;
}

Matrix4R32*
Matrix4R32_createPerspective
  (
    Shizu_State* state,
    Shizu_Float32 fieldOfViewY,
    Shizu_Float32 aspectRatio,
    Shizu_Float32 near,
    Shizu_Float32 far
  )
{
  Matrix4R32* self = (Matrix4R32*)Shizu_Gc_allocate(state, sizeof(Matrix4R32));
  idlib_matrix_4x4_f32_set_perspective(&self->m, fieldOfViewY, aspectRatio, near, far);
  ((Shizu_Object*)self)->type = Matrix4R32_getType(state);
  return self;
}

Matrix4R32*
Matrix4R32_createOrthographic
  (
    Shizu_State* state,
    Shizu_Float32 left,
    Shizu_Float32 right,
    Shizu_Float32 bottom,
    Shizu_Float32 top,
    Shizu_Float32 near,
    Shizu_Float32 far
  )
{
  Matrix4R32* self = (Matrix4R32*)Shizu_Gc_allocate(state, sizeof(Matrix4R32));
  idlib_matrix_4x4_f32_set_orthographic(&self->m, left, right, bottom, top, near, far);
  ((Shizu_Object*)self)->type = Matrix4R32_getType(state);
  return self;
}

Matrix4R32*
Matrix4R32_createLookAt
  (
    Shizu_State* state,
    Vector3R32* source,
    Vector3R32* target,
    Vector3R32* up
  )
{
  Matrix4R32* self = (Matrix4R32*)Shizu_Gc_allocate(state, sizeof(Matrix4R32));
  idlib_matrix_4x4_f32_set_look_at(&self->m, &source->v, &target->v, &up->v);
  ((Shizu_Object*)self)->type = Matrix4R32_getType(state);
  return self;
}

Matrix4R32*
Matrix4R32_createScale
  (
    Shizu_State* state,
    Vector3R32* scale
  )
{
  Matrix4R32* self = (Matrix4R32*)Shizu_Gc_allocate(state, sizeof(Matrix4R32));
  idlib_matrix_4x4_f32_set_scale(&self->m, &scale->v);
  ((Shizu_Object*)self)->type = Matrix4R32_getType(state);
  return self;
}

Matrix4R32*
Matrix4R32_negate
  (
    Shizu_State* state,
    Matrix4R32* matrix
  )
{
  Matrix4R32* self = (Matrix4R32*)Shizu_Gc_allocate(state, sizeof(Matrix4R32));
  idlib_matrix_4x4_f32_negate(&self->m, &matrix->m);
  ((Shizu_Object*)self)->type = Matrix4R32_getType(state);
  return self;
}

Matrix4R32*
Matrix4R32_multiply
  (
    Shizu_State* state,
    Matrix4R32* operand1,
    Matrix4R32* operand2
  )
{
  Matrix4R32* self = (Matrix4R32*)Shizu_Gc_allocate(state, sizeof(Matrix4R32));
  idlib_matrix_4x4_f32_multiply(&self->m, &operand1->m, &operand2->m);
  ((Shizu_Object*)self)->type = Matrix4R32_getType(state);
  return self;
}

#if Zeitgeist_Configuration_CompilerC_Msvc == Zeitgeist_Configuration_CompilerC
  #pragma pop_macro("far")
  #pragma pop_macro("near")
#endif
