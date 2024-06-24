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

#if !defined(MATRIX4R32_H_INCLUDED)
#define MATRIX4R32_H_INCLUDED

#include "Zeitgeist.h"
#include "idlib/math.h"
typedef struct Vector3F32 Vector3F32;

Shizu_declareType(Matrix4F32)

struct Matrix4F32_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Matrix4F32 {
  Shizu_Object _parent;
  idlib_matrix_4x4_f32 m;
};

Matrix4F32*
Matrix4F32_createIdentity
  (
    Shizu_State2* state
  );

Matrix4F32*
Matrix4F32_createTranslate
  (
    Shizu_State2* state,
    Vector3F32* translation
  );

Matrix4F32*
Matrix4F32_createRotateY
  (
    Shizu_State2* state,
    Shizu_Float32 degrees
  );

Matrix4F32*
Matrix4F32_createPerspective
  (
    Shizu_State2* state,
    Shizu_Float32 fieldOfViewY,
    Shizu_Float32 aspectRatio,
    Shizu_Float32 near,
    Shizu_Float32 far
  );

Matrix4F32*
Matrix4F32_createOrthographic
  (
    Shizu_State2* state,
    Shizu_Float32 left,
    Shizu_Float32 right,
    Shizu_Float32 bottom,
    Shizu_Float32 top,
    Shizu_Float32 near,
    Shizu_Float32 far
  );

Matrix4F32*
Matrix4F32_createLookAt
  (
    Shizu_State2* state,
    Vector3F32* source,
    Vector3F32* target,
    Vector3F32* up
  );

Matrix4F32*
Matrix4F32_createScale
  (
    Shizu_State2* state,
    Vector3F32* scale
  );

Matrix4F32*
Matrix4F32_negate
  (
    Shizu_State2* state,
    Matrix4F32* matrix
  );

Matrix4F32*
Matrix4F32_multiply
  (
    Shizu_State2* state,
    Matrix4F32* operand1,
    Matrix4F32* operand2
  );

#endif // MATRIX4R32_H_INCLUDED
