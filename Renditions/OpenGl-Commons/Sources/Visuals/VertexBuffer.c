#include "Visuals/VertexBuffer.h"

static void
Visuals_VertexBuffer_finalize
  (
    Shizu_State* state,
    Visuals_VertexBuffer* self
  );

static void
Visuals_VertexBuffer_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_VertexBuffer_Dispatch* self
  );

static void
Visuals_VertexBuffer_setDataImpl
  (
    Shizu_State* state,
    Visuals_VertexBuffer* self,
    uint8_t flags,
    void const* bytes,
    size_t numberOfBytes
  );

static Shizu_TypeDescriptor const Visuals_VertexBuffer_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_VertexBuffer),
  .finalize = (Shizu_OnFinalizeCallback*)&Visuals_VertexBuffer_finalize,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_VertexBuffer_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_VertexBuffer_dispatchInitialize,
  .dispatchUninitialize = NULL, 
};

Shizu_defineType(Visuals_VertexBuffer, Visuals_Object);

static void
Visuals_VertexBuffer_finalize
  (
    Shizu_State* state,
    Visuals_VertexBuffer* self
  )
{
  if (self->bytes) {
    free(self->bytes);
    self->bytes = NULL;
  }
  self->numberOfBytes = 0;

  self->numberOfVertices = 0;

  self->flags = 0; 
}

static void
Visuals_VertexBuffer_dispatchInitialize
  (
    Shizu_State1* state1,
    Visuals_VertexBuffer_Dispatch* self
  )
{
  self->setData = (void(*)(Shizu_State*, Visuals_VertexBuffer*, uint8_t, void const*, size_t)) & Visuals_VertexBuffer_setDataImpl;
}

static void
Visuals_VertexBuffer_setDataImpl
  (
    Shizu_State* state,
    Visuals_VertexBuffer* self,
    uint8_t flags,
    void const* bytes,
    size_t numberOfBytes
  )
{
  size_t newVertexSize = 0;
  switch (flags) {
    case (Visuals_VertexSemantics_PositionXyz | Visuals_VertexSyntactics_Float3): {
      newVertexSize = sizeof(float) * 3;
    } break;
    case (Visuals_VertexSemantics_PositionXyz_NormalXyz_ColorRgb | Visuals_VertexSyntactics_Float3_Float3_Float3): {
      newVertexSize = sizeof(float) * 9;
    } break;
    default: {
      fprintf(stderr, "%s:%d: unreachable code reached\n", __FILE__, __LINE__);
      Shizu_State_setStatus(state, Shizu_Status_ArgumentInvalid);
      Shizu_State_jump(state);
    } break;
  };
  // Compute new number of Bytes.
  size_t newNumberOfBytes = numberOfBytes;
  // Compute new number of vertices.
  size_t newNumberOfVertices = numberOfBytes / newVertexSize;
  // Warn if this does not hold. 
  if (newNumberOfBytes % newVertexSize) {
    fprintf(stderr, "%s:%d: warning: number of Bytes %zu is not a multiple of the vertex size %zzu\n", __FILE__, __LINE__, newNumberOfBytes, newVertexSize);
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  // Allocate new Bytes.
  void *newBytes = realloc(self->bytes, newNumberOfVertices * newVertexSize > 0 ? newNumberOfVertices * newVertexSize : 1);
  if (!newBytes) {
    Shizu_State_setStatus(state, 1);
    Shizu_State_jump(state);
  }
  memcpy(newBytes, bytes, newNumberOfVertices * newVertexSize);
  // Store the flags.
  self->flags = flags;
  // Store the number of vertices.
  self->numberOfVertices = newNumberOfVertices;
  // Store the bytes and the number of Bytes.
  self->bytes = newBytes;
  self->numberOfBytes = newNumberOfBytes;
}

void
Visuals_VertexBuffer_construct
  (
    Shizu_State* state,
    Visuals_VertexBuffer* self
  )
{
  Shizu_Type* type = Visuals_VertexBuffer_getType(state);
  Visuals_Object_construct(state, (Visuals_Object*)self);
  self->bytes = malloc(sizeof(char));
  if (!self->bytes) {
    Shizu_State_setStatus(state, Shizu_Status_AllocationFailed);
    Shizu_State_jump(state);
  }
  self->numberOfBytes = 0;
  self->numberOfVertices = 0;
  self->flags = Visuals_VertexSemantics_PositionXyz | Visuals_VertexSyntactics_Float3;
  ((Shizu_Object*)self)->type = type;
}
