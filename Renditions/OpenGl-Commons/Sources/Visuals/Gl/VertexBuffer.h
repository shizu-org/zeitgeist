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

#if !defined(VISUALS_GL_VERTEXBUFFER_H_INCLUDED)
#define VISUALS_GL_VERTEXBUFFER_H_INCLUDED

#include "Visuals/VertexBuffer.h"

/**
 * @brief The implementation of Visuals_RenderBuffer for OpenGL.
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
  /// @brief The OpenGL ID of the vertex buffer.
  GLuint bufferId;
  /// @brief The OpenGL ID of the vertex array.
  GLuint vertexArrayId;
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

#endif // VISUALS_GL_VERTEXBUFFER_H_INCLUDED
