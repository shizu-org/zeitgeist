/*
  Zeitgeist
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

#if !defined(VISUALS_VERTEXBUFFER_H_INCLUDED)
#define VISUALS_VERTEXBUFFER_H_INCLUDED

#include "Visuals/Object.h"

/// @brief A vertex representing a position xyz.
#define Visuals_VertexSemantics_PositionXyz (1)
/// @brief Vertices representing a position xyz, a normal xyz, and an ambient rgb color (in that order).
#define Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb (2)
/// @brief Vertices representing a position xyz, a normal xyz, an ambient rgb, a diffuse rgb, a specular rgb, and  shininess.
#define Visuals_VertexSemantics_PositionXyz_NormalXyz_AmbientRgb_DiffuseRgb_SpecularRgb_Shininess (4)

/// @brief A vertex is one vertex element consisting of three float values.
#define Visuals_VertexSyntactics_Float3 (8)
/// @brief A vertex is three vertex elements each consisting of three float values.
#define Visuals_VertexSyntactics_Float3_Float3_Float3 (16)
/// @brief A vertex is 6 vertex elements. The first five vertex element each consist of three float values. The last vertex element consists of a single float value.
#define Visuals_VertexSyntactics_Float3_Float3_Float3_Float3_Float3_Float (32)

/// @since 1.0
/// @brief
/// A vertex buffer comprises
/// - n vertices all of the same vertex semantics and vertex syntactics
/// - the vertex semantics and vertex syntactics of its vertices
/// Currently, the following formats are supported
/// - Visuals_VertexSemantics_PositionXyz | Visuals_VertexSyntactics_Float3
/// - Visuals_VertexSemantics_PositionXyz_NormalXyz_ColorRgb | Visuals_VertexSyntactics_Float3_Float3_Float3
Shizu_declareObjectType(Visuals_VertexBuffer);

struct Visuals_VertexBuffer_Dispatch {
  Visuals_Object_Dispatch _parent;
  void (*setData)(Shizu_State2* state, Visuals_VertexBuffer* self, uint8_t flags, void const* bytes, size_t numberOfBytes);
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
    Shizu_State2* state,
    Visuals_VertexBuffer* self
  );

static inline void
Visuals_VertexBuffer_setData
  (
    Shizu_State2* state,
    Visuals_VertexBuffer* self,
    uint8_t flags,
    void const* bytes,
    size_t numberOfBytes
  )
{ Shizu_VirtualCall(Visuals_VertexBuffer, setData, self, flags, bytes, numberOfBytes); }

#endif // VISUALS_VERTEXBUFFER_H_INCLUDED
