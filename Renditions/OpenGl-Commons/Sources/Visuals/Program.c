#include "Visuals/Program.h"

static void
Visuals_Program_visit
  (
    Shizu_State* state,
    Visuals_Program* self
  )
{
  if (self->vertexProgramSource) {
    Shizu_Gc_visitObject(state, (Shizu_Object*)self->vertexProgramSource);
  }
  if (self->fragmentProgramSource) {
    Shizu_Gc_visitObject(state, (Shizu_Object*)self->fragmentProgramSource);
  }
}

Shizu_TypeDescriptor const Visuals_Program_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .finalize = NULL,
  .visit = (Shizu_OnVisitCallback*)&Visuals_Program_visit,
};

Shizu_defineType(Visuals_Program, Visuals_Object);

void
Visuals_Program_construct
  ( 
    Shizu_State* state,
    Visuals_Program* self,
    Shizu_String* vertexProgramSource,
    Shizu_String* fragmentProgramSource
  )
{
  Shizu_Type* type = Visuals_Program_getType(state);
  Visuals_Object_construct(state, (Visuals_Object*)self);
  self->vertexProgramSource = vertexProgramSource;
  self->fragmentProgramSource = fragmentProgramSource;
  ((Shizu_Object*)self)->type = type;
}

Shizu_TypeDescriptor const Visuals_GlProgram_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .finalize = NULL,
  .visit = NULL,
};

Shizu_defineType(Visuals_GlProgram, Visuals_Program);

void
Visuals_GlProgram_construct
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    Shizu_String* vertexProgramSource,
    Shizu_String* fragmentProgramSource
  )
{
  Shizu_Type* type = Visuals_GlProgram_getType(state);
  Visuals_Program_construct(state, (Visuals_Program*)self, vertexProgramSource, fragmentProgramSource);
  self->vertexProgramId = 0;
  self->fragmentProgramId = 0;
  self->programId = 0;
  ((Shizu_Object*)self)->type = type;
}

Visuals_GlProgram*
Visuals_GlProgram_create
  (
    Shizu_State* state,
    Shizu_String* vertexProgramSource,
    Shizu_String* fragmentProgramSource
  )
{
  Visuals_GlProgram* self = (Visuals_GlProgram*)Shizu_Gc_allocate(state, sizeof(Visuals_GlProgram));
  Visuals_GlProgram_construct(state, self, vertexProgramSource, fragmentProgramSource);
  return self;
}

void
Visuals_GlProgram_materialize
  (
    Shizu_State* state,
    Visuals_GlProgram* self
  )
{
  Shizu_String* temporary = NULL;

  temporary = Shizu_String_concatenate(state, ((Visuals_Program*)self)->vertexProgramSource, Shizu_String_create(state, "", 1));
  self->vertexProgramId = ServiceGl_compileShader(state, GL_VERTEX_SHADER, Shizu_String_getBytes(state, temporary));
 
  temporary = Shizu_String_concatenate(state, ((Visuals_Program*)self)->fragmentProgramSource, Shizu_String_create(state, "", 1));
  self->fragmentProgramId = ServiceGl_compileShader(state, GL_FRAGMENT_SHADER, Shizu_String_getBytes(state, temporary));
  
  self->programId = ServiceGl_linkProgram(state, self->vertexProgramId, self->fragmentProgramId);
}

void
Visuals_GlProgram_unmaterialize
  (
    Shizu_State* state,
    Visuals_GlProgram* self
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
