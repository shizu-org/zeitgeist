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
