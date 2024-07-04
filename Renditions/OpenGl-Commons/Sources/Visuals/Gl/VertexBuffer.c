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
Visuals_Gl_VertexBuffer_finalize
  (
    Shizu_State2* state,
    Visuals_Gl_VertexBuffer* self
  );

static void
Visuals_Gl_VertexBuffer_materializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_VertexBuffer* self
  );

static void
Visuals_Gl_VertexBuffer_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_VertexBuffer* self
  );

static void
Visuals_Gl_VertexBuffer_setDataImpl
  (
    Shizu_State2* state,
    Visuals_Gl_VertexBuffer* self,
    uint8_t flags,
    void const* bytes,
    size_t numberOfBytes
  );

static void
Visuals_Gl_VertexBuffer_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Gl_VertexBuffer_Dispatch* self
  );

static Shizu_ObjectTypeDescriptor const Visuals_Gl_VertexBuffer_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Gl_VertexBuffer),
  .finalize = (Shizu_OnFinalizeCallback*)&Visuals_Gl_VertexBuffer_finalize,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Gl_VertexBuffer_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*) & Visuals_Gl_VertexBuffer_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Visuals_Gl_VertexBuffer, Visuals_VertexBuffer);

static void
Visuals_Gl_VertexBuffer_finalize
  (
    Shizu_State2* state,
    Visuals_Gl_VertexBuffer* self
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
Visuals_Gl_VertexBuffer_materializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_VertexBuffer* self
  )
{
  if (!self->bufferId) {
    while (glGetError()) { }
    glGenBuffers(1, &self->bufferId);
    if (glGetError()) {
      fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenBuffers");
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
    glGenVertexArrays(1, &self->vertexArrayId);
    if (glGetError()) {
      glDeleteBuffers(1, &self->bufferId);
      self->bufferId = 0;
      fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__, "glGenVertexArrays");
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
  }
}

static void
Visuals_Gl_VertexBuffer_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_VertexBuffer* self
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

#define VertexElementSyntax_Float32_1 (1)
#define VertexElementSyntax_Float32_3 (2)
#define VertexElementSemantics_Position (4)
#define VertexElementSemantics_Normal (8)
#define VertexElementSemantics_Ambient (16)
#define VertexElementSemantics_Diffuse (32)
#define VertexElementSemantics_Specular (64)
#define VertexElementSemantics_Shininess (128)

static void
Visuals_Gl_VertexBuffer_setDataImpl
  (
    Shizu_State2* state,
    Visuals_Gl_VertexBuffer* self,
    uint8_t flags,
    void const* bytes,
    size_t numberOfBytes
  )
{
  Shizu_Type* parentType = Shizu_Types_getParentType(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), ((Shizu_Object*)self)->type);
  Visuals_VertexBuffer_Dispatch* parentDispatch = (Visuals_VertexBuffer_Dispatch*)Shizu_Types_getDispatch(Shizu_State2_getState1(state), Shizu_State2_getTypes(state), parentType);
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
    case (Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess | Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float): {
      vertexSize = sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float) * 3 + sizeof(float);
    } break;
    default: {
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
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

  typedef struct VertexElementDesc {
    uint8_t flags;
    size_t size; // The size in Bytes.
    size_t offset; // The offset in Bytes.
  } VertexElementDesc;

  switch (flags) {
    case (Visuals_VertexSemantics_PositionXyz | Visuals_VertexSyntactics_Float3): {
      static const VertexElementDesc vertexElements[] =
      {
        {
          .flags = VertexElementSyntax_Float32_3,
          .offset = 0,
          .size = sizeof(float) * 3,
        },
      };

      size_t offset = 0;

      glEnableVertexAttribArray(POSITION_INDEX);
      glVertexAttribPointer(POSITION_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)offset);
      offset += sizeof(float) * 3;
    } break;
    case (Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb | Visuals_VertexSyntactics_Float3_Float3_Float3): {
      static const VertexElementDesc vertexElements[] =
      {
        {
          .flags = VertexElementSyntax_Float32_3,
          .offset = 0,
          .size = sizeof(float) * 3,
        },
        {
          .flags = VertexElementSyntax_Float32_3,
          .offset = sizeof(float) * 3,
          .size = sizeof(float) * 3,
        },
        {
          .flags = VertexElementSyntax_Float32_3,
          .offset = sizeof(float) * 6,
          .size = sizeof(float) * 3,
        },
      };

      size_t offset = 0;

      glEnableVertexAttribArray(POSITION_INDEX);
      glVertexAttribPointer(POSITION_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)offset);
      offset += sizeof(float) * 3;

      glEnableVertexAttribArray(NORMAL_INDEX);
      glVertexAttribPointer(NORMAL_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)offset);
      offset += sizeof(float) * 3;

      glEnableVertexAttribArray(AMBIENT_COLOR_INDEX);
      glVertexAttribPointer(AMBIENT_COLOR_INDEX,
                            3,
                            GL_FLOAT,
                            GL_TRUE,
                            vertexSize,
                            (void*)(uintptr_t)offset);
      offset += sizeof(float) * 3;
    } break;
    case (Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess | Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float): {
      static const VertexElementDesc vertexElements[] =
      {
        {
          .flags = VertexElementSemantics_Position | VertexElementSyntax_Float32_3,
          .offset = 0,
          .size = sizeof(float) * 3,
        },
        {
          .flags = VertexElementSemantics_Normal | VertexElementSyntax_Float32_3,
          .offset = sizeof(float) * 3,
          .size = sizeof(float) * 3,
        },
        {
          .flags = VertexElementSemantics_Ambient | VertexElementSyntax_Float32_3,
          .offset = sizeof(float) * 6,
          .size = sizeof(float) * 3,
        },
        {
          .flags = VertexElementSemantics_Diffuse | VertexElementSyntax_Float32_3,
          .offset = sizeof(float) * 9,
          .size = sizeof(float) * 3,
        },
        {
          .flags = VertexElementSemantics_Specular | VertexElementSyntax_Float32_3,
          .offset = sizeof(float) * 12,
          .size = sizeof(float) * 3,
        },
        {
          .flags = VertexElementSemantics_Shininess | VertexElementSyntax_Float32_3,
          .offset = sizeof(float) * 15,
          .size = sizeof(float) * 1,
        },
      };

      size_t offset = 0;

      glEnableVertexAttribArray(POSITION_INDEX);
      glVertexAttribPointer(POSITION_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)offset);
      offset += sizeof(float) * 3;

      glEnableVertexAttribArray(NORMAL_INDEX);
      glVertexAttribPointer(NORMAL_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)offset);
      offset += sizeof(float) * 3;

      glEnableVertexAttribArray(AMBIENT_COLOR_INDEX);
      glVertexAttribPointer(AMBIENT_COLOR_INDEX,
                            3,
                            GL_FLOAT,
                            GL_TRUE,
                            vertexSize,
                            (void*)(uintptr_t)offset);
      offset += sizeof(float) * 3;

      glEnableVertexAttribArray(DIFFUSE_COLOR_INDEX);
      glVertexAttribPointer(DIFFUSE_COLOR_INDEX,
                            3,
                            GL_FLOAT,
                            GL_TRUE,
                            vertexSize,
                            (void*)(uintptr_t)offset);
      offset += sizeof(float) * 3;

      glEnableVertexAttribArray(SPECULAR_COLOR_INDEX);
      glVertexAttribPointer(SPECULAR_COLOR_INDEX,
                            3,
                            GL_FLOAT,
                            GL_TRUE,
                            vertexSize,
                            (void*)(uintptr_t)offset);
      offset += sizeof(float) * 3;

      glEnableVertexAttribArray(SHININESSS_INDEX);
      glVertexAttribPointer(SHININESSS_INDEX,
                            1,
                            GL_FLOAT,
                            GL_TRUE,
                            vertexSize,
                            (void*)(uintptr_t)offset);
      offset += sizeof(float) * 1;
    } break;
    default: {
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    } break;
  };

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

static void
Visuals_Gl_VertexBuffer_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_Gl_VertexBuffer_Dispatch* self
  )
{
  ((Visuals_Object_Dispatch*)self)->materialize = (void(*)(Shizu_State2*,Visuals_Object*)) & Visuals_Gl_VertexBuffer_materializeImpl;
  ((Visuals_Object_Dispatch*)self)->unmaterialize = (void(*)(Shizu_State2*,Visuals_Object*)) & Visuals_Gl_VertexBuffer_unmaterializeImpl;
  ((Visuals_VertexBuffer_Dispatch*)self)->setData = (void(*)(Shizu_State2*, Visuals_VertexBuffer*, uint8_t,void const*,size_t)) & Visuals_Gl_VertexBuffer_setDataImpl;
}

void
Visuals_Gl_VertexBuffer_construct
  (
    Shizu_State2* state,
    Visuals_Gl_VertexBuffer* self
  )
{
  Shizu_Type* type = Visuals_Gl_VertexBuffer_getType(state);
  Visuals_VertexBuffer_construct(state, (Visuals_VertexBuffer*)self);
  self->bufferId = 0;
  self->vertexArrayId = 0;
  ((Shizu_Object*)self)->type = type;
}

Visuals_Gl_VertexBuffer*
Visuals_Gl_VertexBuffer_create
  (
    Shizu_State2* state
  )
{
  Visuals_Gl_VertexBuffer* self = (Visuals_Gl_VertexBuffer*)Shizu_Gc_allocateObject(state, sizeof(Visuals_Gl_VertexBuffer));
  Visuals_Gl_VertexBuffer_construct(state, self);
  return self;
}
