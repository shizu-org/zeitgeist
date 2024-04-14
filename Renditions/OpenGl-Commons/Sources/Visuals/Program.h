#if !defined(VISUALS_PROGRAM_H_INCLUDED)
#define VISUALS_PROGRAM_H_INCLUDED

#include "Visuals/Object.h"

/**
 * @since 1.0
 * A program consists of a vertex program and a fragment program.
 */
Shizu_declareType(Zeitgeist_Visuals_Program);
struct Zeitgeist_Visuals_Program {
  Visuals_Object _parent;
  Shizu_String* vertexProgramSource;
  Shizu_String* fragmentProgramSource;
};

void
Zeitgeist_Visuals_Program_construct
  (
    Shizu_State* state,
    Zeitgeist_Visuals_Program* self,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  );

Shizu_declareType(Zeitgeist_Visuals_GlProgram);
struct Zeitgeist_Visuals_GlProgram {
  Zeitgeist_Visuals_Program _parent;
  GLuint vertexProgramId;
  GLuint fragmentProgramId;
  GLuint programId;
};

void
Zeitgeist_Visuals_GlProgram_construct
  (
    Shizu_State* state,
    Zeitgeist_Visuals_GlProgram* self,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  );

Zeitgeist_Visuals_GlProgram*
Zeitgeist_Visuals_GlProgram_create
  (
    Shizu_State* state,
    Shizu_String* vertexSource,
    Shizu_String* fragmentSource
  );

void
Zeitgeist_Visuals_GlProgram_materialize
  (
    Shizu_State* state,
    Zeitgeist_Visuals_GlProgram* self
  );

void
Zeitgeist_Visuals_GlProgram_unmaterialize
  (
    Shizu_State* state,
    Zeitgeist_Visuals_GlProgram* self
  );

#endif // VISUALS_PROGRAM_H_INCLUDED
