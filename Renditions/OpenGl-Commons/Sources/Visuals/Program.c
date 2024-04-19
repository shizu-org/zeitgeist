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

#include "Visuals/Program.h"

static void
Visuals_Program_visit
  (
    Shizu_State* state,
    Visuals_Program* self
  );

Shizu_TypeDescriptor const Visuals_Program_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .size = sizeof(Visuals_Program),
  .finalize = NULL,
  .visit = (Shizu_OnVisitCallback*)&Visuals_Program_visit,
  .dispatchSize = sizeof(Visuals_Program_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Visuals_Program, Visuals_Object);

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

static void
Visuals_GlProgram_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_GlProgram_Dispatch* self
  );

static void
Visuals_GlProgram_materializeImpl
  (
    Shizu_State* state,
    Visuals_GlProgram* self
  );

static void
Visuals_GlProgram_unmaterializeImpl
  (
    Shizu_State* state,
    Visuals_GlProgram* self
  );

Shizu_TypeDescriptor const Visuals_GlProgram_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .size = sizeof(Visuals_GlProgram),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_GlProgram_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_GlProgram_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Visuals_GlProgram, Visuals_Program);

static void
Visuals_GlProgram_materializeImpl
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

static void
Visuals_GlProgram_unmaterializeImpl
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

static void
Visuals_GlProgram_dispatchInitialize
  (            
    Shizu_State1* state1,
    Visuals_GlProgram_Dispatch* self
  )
{
  ((Visuals_Object_Dispatch*)self)->materialize = (void(*)(Shizu_State*, Visuals_Object*)) & Visuals_GlProgram_materializeImpl;
  ((Visuals_Object_Dispatch*)self)->unmaterialize = (void(*)(Shizu_State*, Visuals_Object*)) & Visuals_GlProgram_unmaterializeImpl;
}

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
