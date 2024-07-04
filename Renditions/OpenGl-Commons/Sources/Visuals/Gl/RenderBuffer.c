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

#include "Visuals/Gl/RenderBuffer.h"

static void
Visuals_Gl_RenderBuffer_dispatchInitialize
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer_Dispatch* self
  );

static void
Visuals_Gl_RenderBuffer_finalize
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer* self
  );

static void
Visuals_Gl_RenderBuffer_materializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer* self
  );

static void
Visuals_Gl_RenderBuffer_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer* self
  );

static void
Visuals_Gl_RenderBuffer_resizeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer* self,
    Shizu_Integer32 width,
    Shizu_Integer32 height
  );

static Shizu_ObjectTypeDescriptor const Visuals_Gl_RenderBuffer_Type = {
  .postCreateType = NULL,
  .preDestroyType = NULL,
  .visitType = NULL,
  .size = sizeof(Visuals_Gl_RenderBuffer),
  .finalize = (Shizu_OnFinalizeCallback*)&Visuals_Gl_RenderBuffer_finalize,
  .visit = NULL,
  .dispatchSize = sizeof(Visuals_Gl_RenderBuffer_Dispatch),
  .dispatchInitialize = (Shizu_OnDispatchInitializeCallback*)&Visuals_Gl_RenderBuffer_dispatchInitialize,
  .dispatchUninitialize = NULL,
};

Shizu_defineObjectType(Visuals_Gl_RenderBuffer, Visuals_RenderBuffer);

static void
Visuals_Gl_RenderBuffer_dispatchInitialize
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer_Dispatch* self
  )
{
  ((Visuals_Object_Dispatch*)self)->materialize = (void(*)(Shizu_State2*,Visuals_Object*)) & Visuals_Gl_RenderBuffer_materializeImpl;
  ((Visuals_Object_Dispatch*)self)->unmaterialize = (void(*)(Shizu_State2*,Visuals_Object*)) & Visuals_Gl_RenderBuffer_unmaterializeImpl;
  ((Visuals_RenderBuffer_Dispatch*)self)->resize = (void(*)(Shizu_State2*,Visuals_RenderBuffer*,Shizu_Integer32,Shizu_Integer32)) & Visuals_Gl_RenderBuffer_resizeImpl;
}

static void
Visuals_Gl_RenderBuffer_finalize
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer* self
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
Visuals_Gl_RenderBuffer_materializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer* self
  )
{
  // Create the color attachment texture.
  if (!self->colorTextureId) {
    glGenTextures(1, &self->colorTextureId);
    if (glGetError()) {
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
    glBindTexture(GL_TEXTURE_2D, self->colorTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, self->width, self->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (glGetError()) {
      glDeleteTextures(1, &self->colorTextureId);
      self->colorTextureId = 0;
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
  }

  // Create the depth and stencil attachment texture.
  if (!self->depthStencilTextureId) {
    glGenTextures(1, &self->depthStencilTextureId);
    if (glGetError()) {
      glDeleteTextures(1, &self->colorTextureId);
      self->colorTextureId = 0;
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
    glBindTexture(GL_TEXTURE_2D, self->depthStencilTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, self->width, self->height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (glGetError()) {
      glDeleteTextures(1, &self->depthStencilTextureId);
      self->depthStencilTextureId = 0;
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
  }

  // Create the framebuffer.
  if (!self->frameBufferId) {
    glGenFramebuffers(1, &self->frameBufferId);
    if (glGetError()) {
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, self->frameBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->colorTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, self->depthStencilTextureId, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (glGetError()) {
      glDeleteFramebuffers(1, &self->frameBufferId);
      self->frameBufferId = 0;
      Shizu_State2_setStatus(state, 1);
      Shizu_State2_jump(state);
    }
  }
}

static void
Visuals_Gl_RenderBuffer_unmaterializeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer* self
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
Visuals_Gl_RenderBuffer_resizeImpl
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer* self,
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
Visuals_Gl_RenderBuffer_construct
  (
    Shizu_State2* state,
    Visuals_Gl_RenderBuffer* self
  )
{
  Shizu_Type *TYPE = Visuals_Gl_RenderBuffer_getType(state);
  Visuals_RenderBuffer_construct(state, (Visuals_RenderBuffer*)self);
  self->width = 640;
  self->height = 480;
  self->frameBufferId = 0;
  self->colorTextureId = 0;
  self->depthStencilTextureId = 0;
  ((Shizu_Object*)self)->type = TYPE;
}

Visuals_Gl_RenderBuffer*
Visuals_Gl_RenderBuffer_create
  (
    Shizu_State2* state
  )
{
  Visuals_Gl_RenderBuffer* self = (Visuals_Gl_RenderBuffer*)Shizu_Gc_allocateObject(state, sizeof(Visuals_Gl_RenderBuffer));
  Visuals_Gl_RenderBuffer_construct(state, self);
  return self;
}
