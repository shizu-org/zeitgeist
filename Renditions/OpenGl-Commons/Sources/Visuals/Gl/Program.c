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

#include "Visuals/Gl/Program.h"

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
  
static void
Visuals_GlProgram_bindMatrix4R32Impl
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Matrix4F32* value
  );

static void
Visuals_GlProgram_bindVector3R32Impl
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Vector3F32* value
  );

static void
Visuals_GlProgram_bindVector4R32Impl
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Vector4F32* value
  );

static void
Visuals_GlProgram_bindInteger32Impl
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Shizu_Integer32 value
  );

static void
Visuals_GlProgram_bindBooleanImpl
  ( 
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Shizu_Boolean value
  );

static void
Visuals_GlProgram_bindFloat32Impl
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Shizu_Float32 value
  );

Shizu_TypeDescriptor const Visuals_GlProgram_Type = {
  .preDestroyType = NULL,
  .postCreateType = NULL,
  .visitType = NULL,
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
Visuals_GlProgram_bindMatrix4R32Impl
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Matrix4F32* value
  )
{
  GLint location = glGetUniformLocation(self->programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    glUseProgram(self->programId);
    glUniformMatrix4fv(location, 1, GL_TRUE, idlib_matrix_4x4_f32_get_data(&value->m));
  }
}

static void
Visuals_GlProgram_bindVector3R32Impl
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Vector3F32* value
  )
{
  GLint location = glGetUniformLocation(self->programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    glUseProgram(self->programId);
    glUniform3fv(location, 1, &value->v.e[0]);
  }
}

static void
Visuals_GlProgram_bindVector4R32Impl
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Vector4F32* value
  )
{
  GLint location = glGetUniformLocation(self->programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    glUseProgram(self->programId);
    glUniform4fv(location, 1, &value->v.e[0]);
  }
}

static void
Visuals_GlProgram_bindInteger32Impl
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Shizu_Integer32 value
  )
{
  GLint location = glGetUniformLocation(self->programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    glUseProgram(self->programId);
    glUniform1i(location, value);
  }
}

static void
Visuals_GlProgram_bindBooleanImpl
  ( 
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Shizu_Boolean value
  )
{
  GLint location = glGetUniformLocation(self->programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    glUseProgram(self->programId);
    glUniform1i(location, value ? 1 : 0);
  }
}

static void
Visuals_GlProgram_bindFloat32Impl
  (
    Shizu_State* state,
    Visuals_GlProgram* self,
    char const* name,
    Shizu_Float32 value
  )
{
  GLint location = glGetUniformLocation(self->programId, name);
  if (-1 == location) {
    fprintf(stderr, "%s:%d: unable to get uniform location of uniform `%s`\n", __FILE__, __LINE__, name);
  } else {
    glUseProgram(self->programId);
    glUniform1f(location, value);
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
  ((Visuals_Program_Dispatch*)self)->bindBoolean = (void(*)(Shizu_State*, Visuals_Program*, char const*, Shizu_Boolean)) &Visuals_GlProgram_bindBooleanImpl;
  ((Visuals_Program_Dispatch*)self)->bindFloat32 = (void(*)(Shizu_State*, Visuals_Program*, char const*, Shizu_Float32)) & Visuals_GlProgram_bindFloat32Impl;
  ((Visuals_Program_Dispatch*)self)->bindInteger32 = (void(*)(Shizu_State*, Visuals_Program*, char const*, Shizu_Integer32)) & Visuals_GlProgram_bindInteger32Impl;
  ((Visuals_Program_Dispatch*)self)->bindMatrix4F32 = (void(*)(Shizu_State*, Visuals_Program*, char const*, Matrix4F32 const*)) & Visuals_GlProgram_bindMatrix4R32Impl;
  ((Visuals_Program_Dispatch*)self)->bindVector3F32 = (void(*)(Shizu_State*, Visuals_Program*, char const*, Vector3F32 const*)) & Visuals_GlProgram_bindVector3R32Impl;
  ((Visuals_Program_Dispatch*)self)->bindVector4F32 = (void(*)(Shizu_State*, Visuals_Program*, char const*, Vector4F32 const*)) & Visuals_GlProgram_bindVector4R32Impl;
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
  Visuals_GlProgram* self = (Visuals_GlProgram*)Shizu_Gc_allocateObject(state, sizeof(Visuals_GlProgram));
  Visuals_GlProgram_construct(state, self, vertexProgramSource, fragmentProgramSource);
  return self;
}
