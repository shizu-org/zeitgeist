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

#include "Visuals/Gl/VertexBuffer.h"

static void
Visuals_GlVertexBuffer_finalize
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  );
  
static void
Visuals_GlVertexBuffer_materializeImpl
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  );

static void
Visuals_GlVertexBuffer_unmaterializeImpl
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  );

static void
Visuals_GlVertexBuffer_setDataImpl
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self,
    uint8_t flags,
    void const* bytes,
    size_t numberOfBytes
  );

static void
Visuals_GlVertexBuffer_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_GlVertexBuffer_Dispatch* self
  );

static Shizu_TypeDescriptor const Visuals_GlVertexBuffer_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_GlVertexBuffer),
  .finalize = (Shizu_OnFinalizeCallback*)&Visuals_GlVertexBuffer_finalize,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_GlVertexBuffer_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*) & Visuals_GlVertexBuffer_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineType(Visuals_GlVertexBuffer, Visuals_VertexBuffer);

static void
Visuals_GlVertexBuffer_finalize
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  )
{
	if (self->vertexArrayId) {
		glDeleteVertexArrays(1, &self->vertexArrayId);
		self->vertexArrayId = 0;
	}
	if (self->bufferId) {
		glDeleteBuffers(1, &self->bufferId);
		self->bufferId = 0;
	} 
}

static void
Visuals_GlVertexBuffer_materializeImpl
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  )
{
  if (!self->bufferId) {
    while (glGetError()) { }
    glGenBuffers(1, &self->bufferId);
    if (glGetError()) {
      fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenBuffers");
      Shizu_State_setStatus(state, 1);
      Shizu_State_jump(state);
    }
    glGenVertexArrays(1, &self->vertexArrayId);
    if (glGetError()) {
      glDeleteBuffers(1, &self->bufferId);
      self->bufferId = 0;
      fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenVertexArrays");
      Shizu_State_setStatus(state, 1);
      Shizu_State_jump(state);
    }
  }
}

static void
Visuals_GlVertexBuffer_unmaterializeImpl
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  )
{
  if (self->vertexArrayId) {
    glDeleteVertexArrays(1, &self->vertexArrayId);
    self->vertexArrayId = 0;
  }
  if (self->bufferId) {
    glDeleteBuffers(1, &self->bufferId);
    self->bufferId = 0;
  }
}

static void
Visuals_GlVertexBuffer_setDataImpl
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self,
    uint8_t flags,
    void const* bytes,
    size_t numberOfBytes
  )
{
  Shizu_Type* parentType = Shizu_Types_getParentType(Shizu_State_getState1(state), Shizu_State_getTypes(state), ((Shizu_Object*)self)->type);
  Visuals_VertexBuffer_Dispatch* parentDispatch = (Visuals_VertexBuffer_Dispatch*)Shizu_Types_getDispatch(Shizu_State_getState1(state), Shizu_State_getTypes(state), parentType);
  parentDispatch->setData(state, (Visuals_VertexBuffer*)self, flags, bytes, numberOfBytes);

  static const GLint POSITION_INDEX = 0;
  static const GLint NORMAL_INDEX = 1;
  static const GLint AMBIENT_COLOR_INDEX = 2;
  static const GLint DIFFUSE_COLOR_INDEX = 3;
  static const GLint SPECULAR_COLOR_INDEX = 4;
  static const GLint SHININESSS_INDEX = 5;

  size_t vertexSize = 0;
  switch (((Visuals_VertexBuffer*)self)->flags) {
    case (Visuals_VertexSemantics_PositionXyz | Visuals_VertexSyntactics_Float3): {
      vertexSize = sizeof(float) * 3;
    } break;
    case (Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb | Visuals_VertexSyntactics_Float3_Float3_Float3): {
      vertexSize = sizeof(float) * 9;
    } break;
    default: {
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      Shizu_State_setStatus(state, 1);
      Shizu_State_jump(state);
    } break;
  };

  // Compute number of vertices.
  size_t numberOfVertices = ((Visuals_VertexBuffer*)self)->numberOfVertices;

  Visuals_Object_materialize(state, (Visuals_Object*)self);

  // Store the data in the buffer.
  glBindBuffer(GL_ARRAY_BUFFER, self->bufferId);
  glBufferData(GL_ARRAY_BUFFER, numberOfBytes, bytes, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(self->vertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, self->bufferId);

  switch (flags) {
    case (Visuals_VertexSemantics_PositionXyz | Visuals_VertexSyntactics_Float3): {
      glEnableVertexAttribArray(POSITION_INDEX);
      glVertexAttribPointer(POSITION_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)0);
    } break;
    case (Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb | Visuals_VertexSyntactics_Float3_Float3_Float3): {
      glEnableVertexAttribArray(POSITION_INDEX);
      glVertexAttribPointer(POSITION_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)0);

      glEnableVertexAttribArray(NORMAL_INDEX);
      glVertexAttribPointer(NORMAL_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)(sizeof(float) * 3));

      glEnableVertexAttribArray(AMBIENT_COLOR_INDEX);
      glVertexAttribPointer(AMBIENT_COLOR_INDEX,
                            3,
                            GL_FLOAT,
                            GL_TRUE,
                            vertexSize,
                            (void*)(uintptr_t)(sizeof(float) * 6));
    } break;
    case (Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess | Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float): {
      glEnableVertexAttribArray(POSITION_INDEX);
      glVertexAttribPointer(POSITION_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)0);

      glEnableVertexAttribArray(NORMAL_INDEX);
      glVertexAttribPointer(NORMAL_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)(sizeof(float) * 3));

      glEnableVertexAttribArray(AMBIENT_COLOR_INDEX);
      glVertexAttribPointer(AMBIENT_COLOR_INDEX,
                            3,
                            GL_FLOAT,
                            GL_TRUE,
                            vertexSize,
                            (void*)(uintptr_t)(sizeof(float) * 6));

      glEnableVertexAttribArray(DIFFUSE_COLOR_INDEX);
      glVertexAttribPointer(DIFFUSE_COLOR_INDEX,
                            3,
                            GL_FLOAT,
                            GL_TRUE,
                            vertexSize,
                            (void*)(uintptr_t)(sizeof(float) * 9));

      glEnableVertexAttribArray(SPECULAR_COLOR_INDEX);
      glVertexAttribPointer(SPECULAR_COLOR_INDEX,
                            3,
                            GL_FLOAT,
                            GL_TRUE,
                            vertexSize,
                            (void*)(uintptr_t)(sizeof(float) * 12));

      glEnableVertexAttribArray(SHININESSS_INDEX);
      glVertexAttribPointer(SHININESSS_INDEX,
                            1,
                            GL_FLOAT,
                            GL_TRUE,
                            vertexSize,
                            (void*)(uintptr_t)(sizeof(float) * 15));
    } break;
    default: {
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      Shizu_State_setStatus(state, 1);
      Shizu_State_jump(state);
    } break;
  };

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

static void
Visuals_GlVertexBuffer_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_GlVertexBuffer_Dispatch* self
  )
{
  ((Visuals_Object_Dispatch*)self)->materialize = (void(*)(Shizu_State*,Visuals_Object*)) & Visuals_GlVertexBuffer_materializeImpl;
  ((Visuals_Object_Dispatch*)self)->unmaterialize = (void(*)(Shizu_State*,Visuals_Object*)) & Visuals_GlVertexBuffer_unmaterializeImpl;
  ((Visuals_VertexBuffer_Dispatch*)self)->setData = (void(*)(Shizu_State*, Visuals_VertexBuffer*, uint8_t,void const*,size_t)) & Visuals_GlVertexBuffer_setDataImpl;
}

void
Visuals_GlVertexBuffer_construct
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  )
{
  Shizu_Type* type = Visuals_GlVertexBuffer_getType(state);
  Visuals_VertexBuffer_construct(state, (Visuals_VertexBuffer*)self);
  self->bufferId = 0;
  self->vertexArrayId = 0;
  while (glGetError()) { }
  glGenBuffers(1, &self->bufferId);
  if (glGetError()) {
    fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenBuffers");
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  glGenVertexArrays(1, &self->vertexArrayId);
  if (glGetError()) {
    glDeleteBuffers(1, &self->bufferId);
    self->bufferId = 0;
    fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenVertexArrays");
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  ((Shizu_Object*)self)->type = type;
}

Visuals_GlVertexBuffer*
Visuals_GlVertexBuffer_create
  (
    Shizu_State* state
  )
{
  Visuals_GlVertexBuffer* self = (Visuals_GlVertexBuffer*)Shizu_Gc_allocateObject(state, sizeof(Visuals_GlVertexBuffer));
  Visuals_GlVertexBuffer_construct(state, self);
  return self;
}
