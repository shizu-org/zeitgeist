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

#if !defined(VISUALS_PROGRAM_H_INCLUDED)
#define VISUALS_PROGRAM_H_INCLUDED

#include "Visuals/Object.h"
#include "Matrix4F32.h"
#include "Vector3F32.h"
#include "Vector4F32.h"

/**
 * @since 1.0
 * A program consists of a vertex program and a fragment program.
 */
Shizu_declareType(Visuals_Program);

struct Visuals_Program_Dispatch {
  Visuals_Object_Dispatch _parent;
  void (*bindMatrix4F32)(Shizu_State* state, Visuals_Program* self, char const* name, Matrix4F32* value);
  void (*bindVector3F32)(Shizu_State* state, Visuals_Program* self, char const* name, Vector3F32* value);
  void (*bindVector4F32)(Shizu_State* state, Visuals_Program* self, char const* name, Vector4F32* value);
  void (*bindInteger32)(Shizu_State* state, Visuals_Program* self, char const* name, Shizu_Integer32 value);
  void (*bindFloat32)(Shizu_State* state, Visuals_Program* self, char const* name, Shizu_Float32 value);
  void (*bindBoolean)(Shizu_State* state, Visuals_Program* self, char const* name, Shizu_Boolean value);
};

struct Visuals_Program {
  Visuals_Object _parent;
  Shizu_String* vertexProgramSource;
  Shizu_String* fragmentProgramSource;
};

void
Visuals_Program_construct
  (
    Shizu_State* state,
    Visuals_Program* self,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  );

static inline void
Visuals_Program_bindMatrix4F32
  (
    Shizu_State* state,
    Visuals_Program* self,
    char const* name,
    Matrix4F32* value
  )
{ Shizu_VirtualCall(Visuals_Program, bindMatrix4F32, self, name, value); }

static inline void
Visuals_Program_bindVector3F32
  (
    Shizu_State* state,
    Visuals_Program* self,
    char const* name,
    Vector3F32* value
  )
{ Shizu_VirtualCall(Visuals_Program, bindVector3F32, self, name, value); }

static inline void
Visuals_Program_bindVector4F32
  (
    Shizu_State* state,
    Visuals_Program* self,
    char const* name,
    Vector4F32* value
  )
{ Shizu_VirtualCall(Visuals_Program, bindVector4F32, self, name, value); }

static inline void
Visuals_Program_bindInteger32
  (
    Shizu_State* state,
    Visuals_Program* self,
    char const* name,
    Shizu_Integer32 value
  )
{ Shizu_VirtualCall(Visuals_Program, bindInteger32, self, name, value); }

static inline void
Visuals_Program_bindFloat32
  (
    Shizu_State* state,
    Visuals_Program* self,
    char const* name,
    Shizu_Float32 value
  )
{ Shizu_VirtualCall(Visuals_Program, bindFloat32, self, name, value); }

static inline void
Visuals_Program_bindBoolean
  (
    Shizu_State* state,
    Visuals_Program* self,
    char const* name,
    Shizu_Boolean value
  )
{ Shizu_VirtualCall(Visuals_Program, bindBoolean, self, name, value); }

#endif // VISUALS_PROGRAM_H_INCLUDED
