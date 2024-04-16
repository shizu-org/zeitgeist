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

/**
 * @since 1.0
 * A program consists of a vertex program and a fragment program.
 */
Shizu_declareType(Visuals_Program);

struct Visuals_Program_Dispatch {
  Visuals_Object_Dispatch _parent;
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

Shizu_declareType(Visuals_GlProgram);

struct Visuals_GlProgram_Dispatch {
  Visuals_Program_Dispatch _parent;
};

struct Visuals_GlProgram {
  Visuals_Program _parent;
  GLuint vertexProgramId;
  GLuint fragmentProgramId;
  GLuint programId;
};

void
Visuals_GlProgram_construct
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  );

Visuals_GlProgram*
Visuals_GlProgram_create
  (
    Shizu_State* state,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  );

#endif // VISUALS_PROGRAM_H_INCLUDED
