#if !defined(RENDERBUFFER_H_INCLUDED)
#define RENDERBUFFER_H_INCLUDED

#include "Zeitgeist.h"

#if Shizu_Configuration_OperatingSystem_Windows == Shizu_Configuration_OperatingSystem

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  #include <GL/gl.h>

#elif Shizu_Configuration_OperatingSystem_Linux == Shizu_Configuration_OperatingSystem

  #include <GL/gl.h>

#endif

#include <GL/glext.h>

#define Define(Type, Name) \
  extern Type Name;

#include "ServiceGl_Functions.i"

#undef Define

typedef struct RenderBuffer {
  int width;
  int height;
  GLuint frameBufferId;
  GLuint colorTextureId;
  GLuint depthStencilTextureId;
} RenderBuffer;

#define RenderBuffer_Initializer() { .width = 800, .height = 600, .frameBufferId = 0, .colorTextureId = 0, .depthStencilTextureId = 0 }

// width = 800, height = 600 is typical.
static inline int
RenderBuffer_initialize
  (
    RenderBuffer* self,
    int width,
    int height
  )
{
  self->width = width;
  self->height = height;

  self->frameBufferId = 0;
  self->colorTextureId = 0;
  self->depthStencilTextureId = 0;
  
  // Generate the color attachment texture.
  glGenTextures(1, &self->colorTextureId);
  if (glGetError()) {
     return 1;
  }
  glBindTexture(GL_TEXTURE_2D, self->colorTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (glGetError()) {
     glDeleteTextures(1, &self->colorTextureId);
     self->colorTextureId = 0;
     return 1;
  }
  
  // Generate the depth stencil attachment texture.
  glGenTextures(1, &self->depthStencilTextureId);
  if (glGetError()) {
     glDeleteTextures(1, &self->colorTextureId);
     self->colorTextureId = 0;
     return 1;
  }
  glBindTexture(GL_TEXTURE_2D, self->depthStencilTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  if (glGetError()) {
     glDeleteTextures(1, &self->depthStencilTextureId);
     self->depthStencilTextureId = 0;
     glDeleteTextures(1, &self->colorTextureId);
     self->colorTextureId = 0;
     return 1;
  }
  
  glGenFramebuffers(1, &self->frameBufferId);
  if (glGetError()) {
     glDeleteTextures(1, &self->depthStencilTextureId);
     self->depthStencilTextureId = 0;
     glDeleteTextures(1, &self->colorTextureId);
     self->colorTextureId = 0;
     return 1;
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, self->frameBufferId);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->colorTextureId, 0);   
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, self->depthStencilTextureId, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  if (glGetError()) {
    glDeleteFramebuffers(1, &self->frameBufferId);
    self->frameBufferId = 0;
    glDeleteTextures(1, &self->depthStencilTextureId);
    self->depthStencilTextureId = 0;
    glDeleteTextures(1, &self->colorTextureId);
    self->colorTextureId = 0;
    return 1;
  }

  return 0;
}

static inline int
RenderBuffer_uninitialize
  (
    RenderBuffer* self
  )
{
  //
  if (self->depthStencilTextureId) {
    glDeleteTextures(1, &self->depthStencilTextureId);
    self->depthStencilTextureId = 0;
  }
  //
  if (self->colorTextureId) {
    glDeleteTextures(1, &self->colorTextureId);
    self->colorTextureId = 0;
  }
  //
  if (self->frameBufferId) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &self->frameBufferId);
    self->frameBufferId = 0;
  }
  //
  return 0;
}

static inline int
RenderBuffer_resize
  (
    RenderBuffer* self,
    int width,
    int height
  )
{
 
  if (self->width != width && self->height != height) {
    GLuint colorTextureId, depthStencilTextureId;

    // Generate the color attachment texture.
    glGenTextures(1, &colorTextureId);
    if (glGetError()) {
      return 1;
    }
    glBindTexture(GL_TEXTURE_2D, colorTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Generate the depth attachment texture.
    glGenTextures(1, &depthStencilTextureId);
    if (glGetError()) {
      glDeleteTextures(1, &self->colorTextureId);
      self->colorTextureId = 0;
      return 1;
    }
    glBindTexture(GL_TEXTURE_2D, depthStencilTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // Destroy old depth attachment texture.
    glDeleteTextures(1, &self->depthStencilTextureId);
    // Destroy old color attachment texture.
    glDeleteTextures(1, &self->colorTextureId);

    glBindFramebuffer(GL_FRAMEBUFFER, self->frameBufferId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureId, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilTextureId, 0);
    if (glGetError()) {
      glDeleteTextures(1, &self->depthStencilTextureId);
      self->depthStencilTextureId = 0;
      glDeleteTextures(1, &self->colorTextureId);
      self->colorTextureId = 0;
      return 1;
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      fprintf(stderr, "%s:%d: frame buffer incomplete\n", __FILE__, __LINE__);
      glDeleteTextures(1, &self->depthStencilTextureId);
      self->depthStencilTextureId = 0;
      glDeleteTextures(1, &self->colorTextureId);
      self->colorTextureId = 0;
      return 1;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  
  return 0;
}

static inline int
Renderer_setRenderBuffer
  (
    RenderBuffer* renderBuffer
  )
{
  if (!renderBuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, renderBuffer->frameBufferId);
  }
  return 0;
}

#endif // RENDERBUFFER_H_INCLUDED
