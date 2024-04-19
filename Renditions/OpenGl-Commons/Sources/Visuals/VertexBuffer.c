#include "Visuals/VertexBuffer.h"

static Shizu_TypeDescriptor const Visuals_VertexBuffer_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .size = sizeof(Visuals_VertexBuffer),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_VertexBuffer_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL, 
};

Shizu_defineType(Visuals_VertexBuffer, Visuals_Object);

void
Visuals_VertexBuffer_construct
  (
    Shizu_State* state,
    Visuals_VertexBuffer* self
  )
{
  Shizu_Type* type = Visuals_VertexBuffer_getType(state);
  Visuals_Object_construct(state, (Visuals_Object*)self);
  ((Shizu_Object*)self)->type = type;
}

static void
Visuals_GlVertexBuffer_finalize
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  );
  
static void
Visuals_GlVertexBuffer_materialize
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  );

static void
Visuals_GlVertexBuffer_unmaterialize
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  );

static void
Visuals_GlVertexBuffer_setData
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
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
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
  if (self->bytes) {
    free(self->bytes);
    self->bytes = NULL;
  }
  self->numberOfBytes = 0;
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
Visuals_GlVertexBuffer_materialize
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
Visuals_GlVertexBuffer_unmaterialize
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
Visuals_GlVertexBuffer_setData
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self,
    uint8_t flags,
    void const* bytes,
    size_t numberOfBytes
  )
{
  Visuals_Object_materialize(state, (Visuals_Object*)self);
  static const GLint POSITION_INDEX = 0;

  static const GLint NORMAL_INDEX = 1;

  static const GLint AMBIENT_COLOR_INDEX = 2;

  size_t vertexSize = 0;
  switch (flags) {
    case (Visuals_VertexSemantics_Position3d | Visuals_VertexSyntactics_Float3): {
      vertexSize = sizeof(float) * 3;
    } break;
    case (Visuals_VertexSemantics_Position3d_Normal3d_ColorRgb | Visuals_VertexSyntactics_Float3_Float3_Float3): {
      vertexSize = sizeof(float) * 9;
    } break;
    default: {
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      Shizu_State_setStatus(state, 1);
      Shizu_State_jump(state);
    } break;
  };

  // Compute number of vertices.
  size_t numberOfVertices = numberOfBytes / vertexSize;

  // Warn if this does not hold. 
  if (numberOfBytes % vertexSize) {
    fprintf(stderr, "%s:%d: warning: additional %zu Bytes at the end of the data\n", __FILE__, __LINE__, numberOfBytes % vertexSize);
  }

  // Store the data in the buffer.
  glBindBuffer(GL_ARRAY_BUFFER, self->bufferId);
  self->numberOfBytes = numberOfBytes;
  glBufferData(GL_ARRAY_BUFFER, numberOfBytes, bytes, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(self->vertexArrayId);
  glBindBuffer(GL_ARRAY_BUFFER, self->bufferId);

  switch (flags) {
    case (Visuals_VertexSemantics_Position3d | Visuals_VertexSyntactics_Float3): {
      glEnableVertexAttribArray(POSITION_INDEX);
      glVertexAttribPointer(POSITION_INDEX,
                            3,
                            GL_FLOAT,
                            GL_FALSE,
                            vertexSize,
                            (void*)(uintptr_t)0);
    } break;
    case (Visuals_VertexSemantics_Position3d_Normal3d_ColorRgb | Visuals_VertexSyntactics_Float3_Float3_Float3): {
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
  ((Visuals_Object_Dispatch*)self)->materialize = (void(*)(Shizu_State*,Visuals_Object*)) & Visuals_GlVertexBuffer_materialize;
  ((Visuals_Object_Dispatch*)self)->unmaterialize = (void(*)(Shizu_State*,Visuals_Object*)) & Visuals_GlVertexBuffer_unmaterialize;
  ((Visuals_VertexBuffer_Dispatch*)self)->setData = (void(*)(Shizu_State*, Visuals_VertexBuffer*, uint8_t,void const*,size_t)) & Visuals_GlVertexBuffer_setData;
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
  self->bytes = malloc(sizeof(char));
  if (!self->bytes) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  self->numberOfBytes = 1;
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
  self->numberOfBytes = 1;
  ((Shizu_Object*)self)->type = type;
}

Visuals_GlVertexBuffer*
Visuals_GlVertexBuffer_create
  (
    Shizu_State* state
  )
{
  Visuals_GlVertexBuffer* self = (Visuals_GlVertexBuffer*)Shizu_Gc_allocate(state, sizeof(Visuals_GlVertexBuffer));
  Visuals_GlVertexBuffer_construct(state, self);
  return self;
}
