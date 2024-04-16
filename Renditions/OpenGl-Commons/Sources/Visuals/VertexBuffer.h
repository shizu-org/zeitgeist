#if !defined(VISUALS_VERTEXBUFFER_H_INCLUDED)
#define VISUALS_VERTEXBUFFER_H_INCLUDED

#include "Visuals/Object.h"

#define Visuals_VertexSemantics_Position3d (1)
#define Visuals_VertexSemantics_Position3d_Normal3d_ColorRgb (2)
#define Visuals_VertexSyntactics_Float3 (4)
#define Visuals_VertexSyntactics_Float3_Float3_Float3 (8)

/**
 * @since 1.0
 * A sequence of vertices and the format of each vertex.
 * Furthermore, the interpreation of the sequence as a primitive.
 */
Shizu_declareType(Visuals_VertexBuffer);

struct Visuals_VertexBuffer_Dispatch {
  Visuals_Object_Dispatch _parent;
  void (*setData)(Shizu_State* state, Visuals_VertexBuffer* self, uint8_t flags, void const* bytes, size_t numberOfBytes);
};

struct Visuals_VertexBuffer {
  Visuals_Object parent;
  size_t size;
};

void
Visuals_VertexBuffer_construct
  (
    Shizu_State* state,
    Visuals_VertexBuffer* self
  );

static inline
Visuals_VertexBuffer_setData
  (
    Shizu_State* state,
    Visuals_VertexBuffer* self,
    uint8_t flags,
    void const* bytes,
    size_t numberOfBytes
  )
{
  Visuals_VertexBuffer_Dispatch* dispatch = (Visuals_VertexBuffer_Dispatch*)Shizu_State_getObjectDispatch(state, (Shizu_Object*)self);
  if (!dispatch) {
    fprintf(stderr, "%s:%d: fatal error (unreachable code reached): dispatch not created\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }
  if (dispatch->setData) {
    dispatch->setData(state, self, flags, bytes, numberOfBytes);
  }
}

/**
 * @details
 * bufferId and vertexArrayId are the OpenGL representation of the vertices.
 * bytes and numberOfBytes are shadow copies of the current data.
 * The initial values (until data is specified) bytes and numberOfBytes are a pointer to an array of zero Bytes.
 */
Shizu_declareType(Visuals_GlVertexBuffer);

struct Visuals_GlVertexBuffer_Dispatch {
  Visuals_VertexBuffer_Dispatch _parent;
};

struct Visuals_GlVertexBuffer {
  Visuals_VertexBuffer parent;
  /**
   * @brief The vertex format.
   */
  uint8_t flags;
  /**
   * @brief The OpenGL ID of the vertex buffer.
   */
  GLuint bufferId;
  /**
   * @brief The OpenGL ID of the vertex array.
   */
  GLuint vertexArrayId;
  /**
   * @brief A pointer to an array of @a numberOfBytes Bytes.
   */
  void* bytes;
  /**
   * @brief The number of Bytes in the array pointed to by @a bytes.
   */
  size_t numberOfBytes;
};

void
Visuals_GlVertexBuffer_construct
  (
    Shizu_State* state,
    Visuals_GlVertexBuffer* self
  );

Visuals_GlVertexBuffer*
Visuals_GlVertexBuffer_create
  (
    Shizu_State* state
  );

#endif // VISUALS_VERTEXBUFFER_H_INCLUDED
