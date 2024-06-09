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

#if !defined(VISUALS_GL_PROGRAM_H_INCLUDED)
#define VISUALS_GL_PROGRAM_H_INCLUDED

#include "Visuals/Program.h"
#include "ServiceGl.h"

Shizu_declareType(Visuals_Gl_Program);

struct Visuals_Gl_Program_Dispatch {
  Visuals_Program_Dispatch _parent;
};

struct Visuals_Gl_Program {
  Visuals_Program _parent;
  GLuint vertexProgramId;
  GLuint fragmentProgramId;
  GLuint programId;
};

void
Visuals_Gl_Program_construct
  (
    Shizu_State* state,
    Visuals_Gl_Program* self,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  );

Visuals_Gl_Program*
Visuals_Gl_Program_create
  (
    Shizu_State* state,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  );

#endif // VISUALS_GL_PROGRAM_H_INCLUDED
