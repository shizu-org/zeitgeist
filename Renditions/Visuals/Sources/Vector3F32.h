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

#if !defined(VECTOR3F32_H_INCLUDED)
#define VECTOR3F32_H_INCLUDED

#include "Zeitgeist.h"
#include "idlib/math.h"

/// The type
/// @code
/// class Vector3F32
/// @endcode
/// Its constructor is
/// @code
/// Vector3F32.construct(Float32 x, Float32 y, Float32 z)
/// @endcode
Shizu_declareObjectType(Vector3F32)

struct Vector3F32_Dispatch {
  Shizu_Object_Dispatch _parent;
};

struct Vector3F32 {
  Shizu_Object _parent;
  idlib_vector_3_f32 v;
};

Vector3F32*
Vector3F32_create
  (
    Shizu_State2* state,
    Shizu_Float32 x,
    Shizu_Float32 y,
    Shizu_Float32 z
  );

Vector3F32*
Vector3F32_add
  (
    Shizu_State2* state,
    Vector3F32* left,
    Vector3F32* right
  );

Vector3F32*
Vector3F32_subtract
  (
    Shizu_State2* state,
    Vector3F32* left,
    Vector3F32* right
  );

#endif // VECTOR3F32_H_INCLUDED
