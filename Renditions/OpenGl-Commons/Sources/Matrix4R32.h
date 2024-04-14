#if !defined(MATRIX4R32_H_INCLUDED)
#define MATRIX4R32_H_INCLUDED

#include "Zeitgeist.h"
#include "idlib-math.h"
typedef struct Vector3R32 Vector3R32;

Shizu_declareType(Matrix4R32)

struct Matrix4R32 {
  Shizu_Object _parent;
  idlib_matrix_4x4_f32 m;
};

Matrix4R32*
Matrix4R32_createIdentity
  (
    Shizu_State* state
  );

Matrix4R32*
Matrix4R32_createTranslate
  (
    Shizu_State* state,
    Vector3R32* translation
  );

Matrix4R32*
Matrix4R32_createRotateY
  (
    Shizu_State* state,
    Shizu_Float32 degrees
  );

Matrix4R32*
Matrix4R32_createPerspective
  (
    Shizu_State* state,
    Shizu_Float32 fieldOfViewY,
    Shizu_Float32 aspectRatio,
    Shizu_Float32 near,
    Shizu_Float32 far
  );

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
  );

Matrix4R32*
Matrix4R32_createLookAt
  (
    Shizu_State* state,
    Vector3R32* source,
    Vector3R32* target,
    Vector3R32* up
  );

Matrix4R32*
Matrix4R32_createScale
  (
    Shizu_State* state,
    Vector3R32* scale
  );

Matrix4R32*
Matrix4R32_negate
  (
    Shizu_State* state,
    Matrix4R32* matrix
  );

Matrix4R32*
Matrix4R32_multiply
  (
    Shizu_State* state,
    Matrix4R32* operand1,
    Matrix4R32* operand2
  );

#endif // MATRIX4R32_H_INCLUDED
