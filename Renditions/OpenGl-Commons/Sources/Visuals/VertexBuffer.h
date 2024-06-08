#if !defined(VISUALS_VERTEXBUFFER_H_INCLUDED)
#define VISUALS_VERTEXBUFFER_H_INCLUDED

#include "Visuals/Object.h"

/**
 * @brief A vertex representing a position xyz.
 */
#define Visuals_VertexSemantics_PositionXyz (1)
/**
 * @brief Vertices representing a position xyz, a normal xyz, and a color rgb (in that order).
 */
#define Visuals_VertexSemantics_PositionXyz_NormalXyz_ColorRgb (2)

/**
 * @brief A vertex is one vertex element consisting of three float values. 
 */
#define Visuals_VertexSyntactics_Float3 (4)

/** 
 * @brief A vertex is three vertex elements each consisting of three float values.
 */
#define Visuals_VertexSyntactics_Float3_Float3_Float3 (8)

/**
 * @since 1.0
 * @brief
 * A vertex buffer comprises
 * - n vertices all of the same vertex semantics and vertex syntactics
 * - the vertex semantics and vertex syntactics of its vertices
 * Currently, the following formats are supported
 * - Visuals_VertexSemantics_PositionXyz | Visuals_VertexSyntactics_Float3
 * - Visuals_VertexSemantics_PositionXyz_NormalXyz_ColorRgb | Visuals_VertexSyntactics_Float3_Float3_Float3
 */
Shizu_declareType(Visuals_VertexBuffer);

struct Visuals_VertexBuffer_Dispatch {
  Visuals_Object_Dispatch _parent;
  void (*setData)(Shizu_State* state, Visuals_VertexBuffer* self, uint8_t flags, void const* bytes, size_t numberOfBytes);
};

struct Visuals_VertexBuffer {
  Visuals_Object parent;
  /// @brief The number of vertices.
  size_t numberOfVertices;
  /// @brief The vertex format.
  uint8_t flags;
  /// @brief A pointer to an array of @a numberOfBytes Bytes.
  void* bytes;
  /// @brief The number of Bytes in the array pointed to by @a bytes.
  size_t numberOfBytes;
};

/**
 * @brief Construct a vertex buffer with default values.
 * The default values are
 * - zero vertices
 * - of the format Visuals_VertexSemantics_PositionXyz | Visuals_VertexSyntactics_Float3
 */
void
Visuals_VertexBuffer_construct
  (
    Shizu_State* state,
    Visuals_VertexBuffer* self
  );

static inline void
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

#endif // VISUALS_VERTEXBUFFER_H_INCLUDED
