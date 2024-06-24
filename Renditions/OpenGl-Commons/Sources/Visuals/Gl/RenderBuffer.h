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

#if !defined(VISUALS_GL_RENDERBUFFER_H_INCLUDED)
#define VISUALS_GL_RENDERBUFFER_H_INCLUDED

#include "Visuals/RenderBuffer.h"
#include "ServiceGl.h"

Shizu_declareType(Visuals_Gl_RenderBuffer);

struct Visuals_Gl_RenderBuffer_Dispatch {
  Visuals_RenderBuffer_Dispatch _parent;
};

struct Visuals_Gl_RenderBuffer {
  Visuals_RenderBuffer _parent;
  Shizu_Integer32 width;
  Shizu_Integer32 height;
  GLuint frameBufferId;
  GLuint colorTextureId;
  GLuint depthStencilTextureId;
};

void
Visuals_Gl_RenderBuffer_construct
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer* self
  );

Visuals_Gl_RenderBuffer*
Visuals_Gl_RenderBuffer_create
  (
    Shizu_State2* state
  );

#endif // VISUALS_GL_RENDERBUFFER_H_INCLUDED
