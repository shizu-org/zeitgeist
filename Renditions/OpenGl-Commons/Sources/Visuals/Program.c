#include "Visuals/Program.h"

static void
Zeitgeist_Visuals_Program_visit
  (
    Shizu_State* state,
    Zeitgeist_Visuals_Program* self
  )
{
  if (self->vertexProgramSource) {
    Shizu_Gc_visitObject(state, (Shizu_Object*)self->vertexProgramSource);
  }
  if (self->fragmentProgramSource) {
    Shizu_Gc_visitObject(state, (Shizu_Object*)self->fragmentProgramSource);
  }
}

static Shizu_TypeDescriptor const Zeitgeist_Visuals_Program_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .finalize = NULL,
  .visit = (Shizu_OnVisitCallback*)&Zeitgeist_Visuals_Program_visit,
};

Shizu_defineType(Zeitgeist_Visuals_Program, Visuals_Object);

void
Zeitgeist_Visuals_Program_construct
  ( 
    Shizu_State* state,
    Zeitgeist_Visuals_Program* self,
    Shizu_String* vertexProgramSource,
    Shizu_String* fragmentProgramSource
  )
{
  Shizu_Type* type = Zeitgeist_Visuals_Program_getType(state);
  Visuals_Object_construct(state, (Visuals_Object*)self);
  self->vertexProgramSource = vertexProgramSource;
  self->fragmentProgramSource = fragmentProgramSource;
  ((Shizu_Object*)self)->type = type;
}

static Shizu_TypeDescriptor const Zeitgeist_Visuals_GlProgram_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .finalize = NULL,
  .visit = NULL,
};

Shizu_defineType(Zeitgeist_Visuals_GlProgram, Zeitgeist_Visuals_Program);

void
Zeitgeist_Visuals_GlProgram_construct
  (
    Shizu_State* state,
    Zeitgeist_Visuals_GlProgram* self,
    Shizu_String* vertexProgramSource,
    Shizu_String* fragmentProgramSource
  )
{
  Shizu_Type* type = Zeitgeist_Visuals_GlProgram_getType(state);
  Zeitgeist_Visuals_Program_construct(state, (Zeitgeist_Visuals_Program*)self, vertexProgramSource, fragmentProgramSource);
  self->vertexProgramId = 0;
  self->fragmentProgramId = 0;
  self->programId = 0;
  ((Shizu_Object*)self)->type = type;
}

Zeitgeist_Visuals_GlProgram*
Zeitgeist_Visuals_GlProgram_create
  (
    Shizu_State* state,
    Shizu_String* vertexProgramSource,
    Shizu_String* fragmentProgramSource
  )
{
  Zeitgeist_Visuals_GlProgram* self = (Zeitgeist_Visuals_GlProgram*)Shizu_Gc_allocate(state, sizeof(Zeitgeist_Visuals_GlProgram));
  Zeitgeist_Visuals_GlProgram_construct(state, self, vertexProgramSource, fragmentProgramSource);
  return self;
}


void
Zeitgeist_Visuals_GlProgram_materialize
  (
    Shizu_State* state,
    Zeitgeist_Visuals_GlProgram* self
  )
{
  Shizu_String* temporary = NULL;

  temporary = Shizu_String_concatenate(state, ((Zeitgeist_Visuals_Program*)self)->vertexProgramSource, Shizu_String_create(state, "", 1));
  self->vertexProgramId = ServiceGl_compileShader(state, GL_VERTEX_SHADER, Shizu_String_getBytes(state, temporary));
 
  temporary = Shizu_String_concatenate(state, ((Zeitgeist_Visuals_Program*)self)->fragmentProgramSource, Shizu_String_create(state, "", 1));
  self->fragmentProgramId = ServiceGl_compileShader(state, GL_FRAGMENT_SHADER, Shizu_String_getBytes(state, temporary));
  
  self->programId = ServiceGl_linkProgram(state, self->vertexProgramId, self->fragmentProgramId);
}

void
Zeitgeist_Visuals_GlProgram_unmaterialize
  (
    Shizu_State* state,
    Zeitgeist_Visuals_GlProgram* self
  )
{
  if (self->vertexProgramId) {
    glDeleteShader(self->vertexProgramId);
    self->vertexProgramId = 0;
  }
  if (self->fragmentProgramId) {
    glDeleteShader(self->fragmentProgramId);
    self->fragmentProgramId = 0;
  }
  if (self->programId) {
    glDeleteProgram(self->programId);
    self->programId = 0;
  }
}
