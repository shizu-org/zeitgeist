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

#include "Visuals/RenderBuffer.h"

static Shizu_TypeDescriptor const Visuals_RenderBuffer_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .size = sizeof(Visuals_RenderBuffer),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_RenderBuffer_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL, 
};

Shizu_defineType(Visuals_RenderBuffer, Visuals_Object);

void
Visuals_RenderBuffer_construct
  (
    Shizu_State* state,
    Visuals_RenderBuffer* self
  )
{
  Shizu_Type* type = Visuals_RenderBuffer_getType(state);
  ((Shizu_Object*)self)->type = type;
}

static void
Visuals_GlRenderBuffer_dispatchInitialize
  (
    Shizu_State* state,
    Visuals_GlRenderBuffer_Dispatch* self
  );

static void
Visuals_GlRenderBuffer_materializeImpl
  (
    Shizu_State* state,
    Visuals_GlRenderBuffer* self
  );

static void
Visuals_GlRenderBuffer_unmaterializeImpl
  (
    Shizu_State* state,
    Visuals_GlRenderBuffer* self
  );

static void
Visuals_GlRenderBuffer_resizeImpl
  (   
    Shizu_State* state,
    Visuals_GlRenderBuffer* self,
    Shizu_Integer32 width,
    Shizu_Integer32 height
  );

static Shizu_TypeDescriptor const Visuals_GlRenderBuffer_Type = {
  .staticInitialize = NULL,
  .staticFinalize = NULL,
  .staticVisit = NULL,
  .size = sizeof(Visuals_GlRenderBuffer),
  .finalize = NULL,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_GlRenderBuffer_Dispatch),
  .dispatchInitialize = NULL,
  .dispatchUninitialize = NULL, 
};

Shizu_defineType(Visuals_GlRenderBuffer, Visuals_RenderBuffer);

static void
Visuals_GlRenderBuffer_dispatchInitialize
  (
    Shizu_State* state,
    Visuals_GlRenderBuffer_Dispatch* self
  )
{ 
  ((Visuals_Object_Dispatch*)self)->materialize = (void(*)(Shizu_State*,Visuals_Object*)) & Visuals_GlRenderBuffer_materializeImpl;
  ((Visuals_Object_Dispatch*)self)->unmaterialize = (void(*)(Shizu_State*,Visuals_Object*)) & Visuals_GlRenderBuffer_unmaterializeImpl;
  ((Visuals_RenderBuffer_Dispatch*)self)->resize = (void(*)(Shizu_State*,Visuals_RenderBuffer*,Shizu_Integer32,Shizu_Integer32)) & Visuals_GlRenderBuffer_resizeImpl;
}

static void
Visuals_GlRenderBuffer_materializeImpl
  (
    Shizu_State* state,
    Visuals_GlRenderBuffer* self
  )
{
  // Create the color attachment texture.
  if (!self->colorTextureId) {
    glGenTextures(1, &self->colorTextureId);
    if (glGetError()) {
      Shizu_State_setError(state, 1);
      Shizu_State_jump(state);
    }
    glBindTexture(GL_TEXTURE_2D, self->colorTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, self->width, self->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (glGetError()) {
      glDeleteTextures(1, &self->colorTextureId);
      self->colorTextureId = 0;
      Shizu_State_setError(state, 1);
      Shizu_State_jump(state);
    }
  }

  // Create the depth and stencil attachment texture.
  if (!self->depthStencilTextureId) {
    glGenTextures(1, &self->depthStencilTextureId);
    if (glGetError()) {
      glDeleteTextures(1, &self->colorTextureId);
      self->colorTextureId = 0;
      Shizu_State_setError(state, 1);
      Shizu_State_jump(state);
    }
    glBindTexture(GL_TEXTURE_2D, self->depthStencilTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, self->width, self->height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (glGetError()) {
      glDeleteTextures(1, &self->depthStencilTextureId);
      self->depthStencilTextureId = 0;
      Shizu_State_setError(state, 1);
      Shizu_State_jump(state);
    }
  }

  // Create the framebuffer.
  if (!self->frameBufferId) {
    glGenFramebuffers(1, &self->frameBufferId);
    if (glGetError()) {
      Shizu_State_setError(state, 1);
      Shizu_State_jump(state);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, self->frameBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->colorTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, self->depthStencilTextureId, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (glGetError()) {
      glDeleteFramebuffers(1, &self->frameBufferId);
      self->frameBufferId = 0;
      Shizu_State_setError(state, 1);
      Shizu_State_jump(state);
    }
  }
}

static void
Visuals_GlRenderBuffer_unmaterializeImpl
  (
    Shizu_State* state,
    Visuals_GlRenderBuffer* self
  )
{
  // Destroy the framebuffer.
  if (self->frameBufferId) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &self->frameBufferId);
    self->frameBufferId = 0;
  }
  // Destroy the depth and stencil attachment texture.
  if (self->depthStencilTextureId) {
    glDeleteTextures(1, &self->depthStencilTextureId);
    self->depthStencilTextureId = 0;
  }
  // Destroy the color attachment texture.
  if (self->colorTextureId) {
    glDeleteTextures(1, &self->colorTextureId);
    self->colorTextureId = 0;
  }
}

static void
Visuals_GlRenderBuffer_resizeImpl
  (
    Shizu_State* state,
    Visuals_GlRenderBuffer* self,
    Shizu_Integer32 width,
    Shizu_Integer32 height
  )
{
  if (self->width != width || self->height != height) {
    Visuals_Object_unmaterialize(state, (Visuals_Object*)self);
    self->width = width;
    self->height = height;
    Visuals_Object_materialize(state, (Visuals_Object*)self);
  }
}

void
Visuals_GlRenderBuffer_construct
  (
    Shizu_State* state,
    Visuals_GlRenderBuffer* self
  )
{
  Shizu_Type* type = Visuals_GlRenderBuffer_getType(state);
  ((Shizu_Object*)self)->type = type;
}

Visuals_GlRenderBuffer*
Visuals_GlRenderBuffer_create
  (
    Shizu_State* state
  )
{
  Visuals_GlRenderBuffer* self = (Visuals_GlRenderBuffer*)Shizu_Gc_allocate(state, sizeof(Visuals_GlRenderBuffer));
  Visuals_GlRenderBuffer_construct(state, self);
  return self;  
}
