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

#if !defined(VISUALS_RENDERBUFFER_H_INCLUDED)
#define VISUALS_RENDERBUFFER_H_INCLUDED

#include "Visuals/Object.h"

/**
 * @since 1.0
 * A render buffer.
 */
Shizu_declareType(Visuals_RenderBuffer);

struct Visuals_RenderBuffer_Dispatch {
  Visuals_Object_Dispatch _parent;
  void (*resize)(Shizu_State* state, Visuals_RenderBuffer* self, Shizu_Integer32 width, Shizu_Integer32 height);
};

struct Visuals_RenderBuffer {
  Visuals_Object _parent;
};

void
Visuals_RenderBuffer_construct
  (
    Shizu_State* state,
    Visuals_RenderBuffer* self
  );

static inline void
Visuals_RenderBuffer_resize
  (    
    Shizu_State* state,
    Visuals_RenderBuffer* self,
    Shizu_Integer32 width,
    Shizu_Integer32 height
  )
{
  Visuals_RenderBuffer_Dispatch* dispatch = (Visuals_RenderBuffer_Dispatch*)Shizu_State_getObjectDispatch(state, (Shizu_Object*)self);
  if (!dispatch) {
    fprintf(stderr, "%s:%d: fatal error (unreachable code reached): dispatch not created\n", __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }
  if (dispatch->resize) {
    dispatch->resize(state, self, width, height);
  }
}

Shizu_declareType(Visuals_GlRenderBuffer);

struct Visuals_GlRenderBuffer_Dispatch {
  Visuals_RenderBuffer_Dispatch _parent;
};

struct Visuals_GlRenderBuffer {
  Visuals_RenderBuffer _parent;
  Shizu_Integer32 width;
  Shizu_Integer32 height;
  GLuint frameBufferId;
  GLuint colorTextureId;
  GLuint depthStencilTextureId;
};

void
Visuals_GlRenderBuffer_construct
  (
    Shizu_State* state,
    Visuals_GlRenderBuffer* self
  );

Visuals_GlRenderBuffer*
Visuals_GlRenderBuffer_create
  (
    Shizu_State* state
  );

#endif // VISUALS_RENDERBUFFER_H_INCLUDED
